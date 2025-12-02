
# Upgraded Bus Reservation — UI Upgrade (Option 1)

This project upgrades the Streamlit UI with a modern, colorful design and an interactive seat layout.
C++ binary (`reservation`) handles booking logic and persists seat state in `seats.dat` and `bookings.csv`.

## Files
- `reservation.cpp` — C++ backend (persistent, file-based)
- `app.py` — Streamlit frontend (improved UI + seat grid)
- `style.css` — Custom styles for Streamlit

## How to run

1. Install dependencies:
   ```bash
   pip install streamlit
   ```

2. Compile the C++ program:
   ```bash
   g++ reservation.cpp -o reservation
   chmod +x reservation
   ```

3. Run Streamlit:
   ```bash
   streamlit run app.py
   ```

Open the browser at the URL Streamlit provides (usually http://localhost:8501).

Notes:
- The C++ binary persists seats in `seats.dat` (20 seats: 0 = free, 1 = booked).
- Bookings are appended to `bookings.csv`.
- Passenger names with spaces are converted (Streamlit replaces spaces for the C++ call).

