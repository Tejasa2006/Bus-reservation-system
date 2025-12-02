

# app.py
# Streamlit frontend for advanced bus reservation with multiple buses
# Usage: make sure `reservation` binary is compiled and in same folder.

import streamlit as st
import subprocess, json, shlex
from datetime import datetime

st.set_page_config(page_title="ProBus — Reservation", layout="wide", page_icon="🚌")

# load css
with open("style.css") as f:
    st.markdown(f"<style>{f.read()}</style>", unsafe_allow_html=True)

def call_cpp(args):
    # returns parsed JSON or raw dict
    try:
        cmd = ["./reservation", *args]
        # windows fallback
        proc = subprocess.run(cmd, capture_output=True, text=True)
        out = proc.stdout.strip()
        if not out:
            out = proc.stderr.strip()
        try:
            return json.loads(out)
        except:
            # attempt to replace single quotes
            try:
                return json.loads(out.replace("'", "\""))
            except:
                return {"raw": out}
    except Exception as e:
        return {"error": str(e)}

st.title("🚌 ProBus — Realistic Bus Reservation System")

tabs = st.tabs(["Search & Book", "My Bookings", "Admin"])

# --- Search & Book tab
with tabs[0]:
    st.header("Search Buses")
    col1, col2, col3 = st.columns([3,2,1])
    with col1:
        frm = st.text_input("From", value="CityA")
        to = st.text_input("To", value="CityB")
    with col2:
        date = st.date_input("Travel Date")
    with col3:
        if st.button("Search", key="search_btn"):
            res = call_cpp(["search", frm, to, date.strftime("%Y-%m-%d")])
            st.session_state['search_results'] = res.get("buses", [])

    buses = st.session_state.get('search_results', [])

    # ---- Bus List -----
    if buses:
        for i, b in enumerate(buses):
            st.markdown("---")
            cols = st.columns([2,3,2,1])
            with cols[0]:
                st.markdown(f"**{b.get('number')}**")
                st.markdown(f"{b.get('from')} → {b.get('to')}")
            with cols[1]:
                st.markdown(f"Date: {b.get('date')} • Departure: {b.get('depart_time')}")
                st.markdown(f"Type: {b.get('type')} • Seats: {b.get('seats')}")
            with cols[2]:
                st.markdown(f"Fare: ₹{b.get('fare')}")
            with cols[3]:
                # UNIQUE KEY FIX
                if st.button(f"View Seats", key=f"view_{i}_{b.get('id')}_{b.get('number')}"):
                    st.session_state["sel_bus"] = b
                    st.session_state["view_seats"] = call_cpp(["seats", b.get('id')])
                    st.session_state.pop("selected_seat", None)
                    st.rerun()

    # ---- Seat Layout -----
    if st.session_state.get("view_seats"):
        sel_bus = st.session_state["sel_bus"]
        vr = st.session_state["view_seats"]
        seats = vr.get("seats", [])

        st.subheader(f"Seats — {sel_bus['number']} ({sel_bus['from']} → {sel_bus['to']})")

        cols = st.columns(5)
        idx = 1
        for r in range((len(seats)+4)//5):
            for c in range(5):
                if idx <= len(seats):
                    status = seats[idx-1]
                    label = f"{idx}"

                    # UNIQUE KEY FIX
                    skey = f"seat_{sel_bus['id']}_{sel_bus['number']}_{idx}"

                    if status == 1:
                        cols[c].button(label + " ❌", key=skey+"_booked")
                    else:
                        if cols[c].button(label + " ✅", key=skey+"_free"):
                            st.session_state["selected_seat"] = idx
                            st.rerun()

                    idx += 1

        st.markdown("### Booking Details")
        name = st.text_input("Passenger name", key="pn_input")
        phone = st.text_input("Phone", key="ph_input")

        selected = st.session_state.get("selected_seat")
        st.markdown(f"**Selected seat:** {selected if selected else '-'}")

        if st.button("Book Ticket", key="book_ticket_btn"):
            if not selected:
                st.error("Please select a seat.")
            elif not name or not phone:
                st.error("Please enter both name and phone.")
            else:
                safe_name = name.replace(" ", "_")
                safe_phone = phone.replace(" ", "")

                res = call_cpp(["book", sel_bus["id"], str(selected), safe_name, safe_phone])
                if res.get("success"):
                    st.success(f"Booked! PNR {res['pnr']} • Seat {res['seat']} • ₹{res['fare']}")
                    st.session_state["view_seats"] = call_cpp(["seats", sel_bus['id']])
                    st.session_state.pop("selected_seat", None)
                    st.rerun()
                else:
                    st.error(res.get("error", "Booking failed."))

# --- My Bookings tab
# --- My Bookings tab
with tabs[1]:
    st.header("All Bookings")

    res = call_cpp(["bookings"])
    bookings = res.get("bookings", [])

    if bookings:
        # Reverse list once
        bookings = bookings[::-1]

        for i, b in enumerate(bookings):
            pnr = b.get("pnr") or f"PNR_MISSING_{i}"
            name = b.get("name", "Unknown")
            seat = b.get("seat", "-")
            busid = b.get("busid", "-")
            dt = b.get("datetime", "-")

            st.markdown(
                f"**PNR:** {pnr}  |  {name}  |  Seat {seat}  |  Bus: {busid}  |  {dt}"
            )

            if st.button(f"Cancel {pnr}", key=f"cancel_{pnr}_{i}"):
                cres = call_cpp(["cancel", pnr])
                if cres.get("success"):
                    st.success(f"Cancelled booking {pnr}")
                else:
                    st.error(cres.get("error", "Cancel failed"))
                st.experimental_rerun()
    else:
        st.info("No bookings found.")


# --- Admin tab
with tabs[2]:
    st.header("Admin — Manage Buses")
    st.markdown("### Add Bus")
    with st.form("addbus"):
        number = st.text_input("Bus number (e.g. PB10AB1234)")
        frm = st.text_input("From")
        to = st.text_input("To")
        date = st.date_input("Date")
        depart = st.text_input("Departure (HH:MM)", value="09:00")
        btype = st.selectbox("Type", ["AC", "Non-AC", "Sleeper"])
        seats = st.number_input("Seats", min_value=10, max_value=60, value=20)
        fare = st.number_input("Fare (₹)", min_value=50.0, value=200.0)
        submitted = st.form_submit_button("Add Bus")
        if submitted:
            res = call_cpp(["admin_addbus", number, frm, to, date.strftime("%Y-%m-%d"), depart, btype, str(int(seats)), str(float(fare))])
            if res.get("success"):
                st.success(f"Added bus {res.get('id')}")
                st.rerun()
            else:
                st.error(res.get("error", "failed"))

    st.markdown("### Existing Buses")

lb = call_cpp(["list_buses"])
buses = lb.get("buses", [])

if buses:
    for i, b in enumerate(buses):

        st.markdown(
            f"- **{b.get('number')}** ({b.get('from')}→{b.get('to')}) • "
            f"{b.get('date')} {b.get('depart_time')} • {b.get('type')} • "
            f"Seats: {b.get('seats')} • Fare: ₹{b.get('fare')}"
        )

        cols = st.columns([1, 1, 1])

        if cols[0].button(f"Remove {b.get('id')}", key=f"rm_{i}_{b.get('id')}"):
            call_cpp(["admin_removebus", b.get('id')])
            st.rerun()

        if cols[1].button(f"Increase seats {b.get('id')}", key=f"inc_{i}_{b.get('id')}"):
            newseats = int(b.get('seats')) + 5
            call_cpp(["admin_updatebus", b.get('id'), "seats", str(newseats)])
            st.rerun()

        if cols[2].button(f"Decrease seats {b.get('id')}", key=f"dec_{i}_{b.get('id')}"):
            newseats = max(10, int(b.get('seats')) - 5)
            call_cpp(["admin_updatebus", b.get('id'), "seats", str(newseats)])
            st.rerun()

