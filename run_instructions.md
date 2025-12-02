
# Bus Reservation System — UI Upgrade (C++ logic + Streamlit frontend)

This archive contains a polished Streamlit frontend that calls a C++ backend binary for booking logic.
Everything runs locally — no server deployment required.

## Included files
- `reservation.cpp` — C++ backend (persists seats in `seats.dat`, appends bookings to `bookings.csv`)
- `app.py` — Streamlit frontend (modern UI, seat grid, stats, bookings list)
- `style.css` — Custom CSS for Streamlit
- `README.txt` — Short notes (included previously)
- `run_instructions.md` — Step-by-step run instructions (this file)

---

## Prerequisites

### On Linux / macOS
- Python 3.8+ installed (`python3`)
- `pip` available
- `g++` (GNU C++ compiler)
- (Optional) `make` if you like using Makefiles

### On Windows (recommended using WSL for easiest compatibility)
- Install Python 3.8+ from python.org
- Install MinGW or use WSL with g++ available
- Alternatively use Visual Studio's `cl` compiler (commands below assume g++)

---

## Setup & Run — Step-by-step (Linux / macOS)

1. Open a terminal and navigate to the folder where you extracted the zip:
```bash
cd /path/to/extracted/folder
ls
# you should see: reservation.cpp  app.py  style.css  README.txt
```

2. (Optional) Create a Python virtual environment:
```bash
python3 -m venv venv
source venv/bin/activate
```

3. Install required Python package (Streamlit):
```bash
pip install streamlit
```

4. Compile the C++ program (uses g++):
```bash
g++ reservation.cpp -o reservation
```
- If you see permission errors later, run:
```bash
chmod +x reservation
```

5. Run the Streamlit app:
```bash
streamlit run app.py
```
- Streamlit will start and print a local URL (usually `http://localhost:8501`). Open that in your browser.

6. Use the UI:
- Click seats in the seat layout to select.
- Enter passenger name in the sidebar and click **Book Selected** to book.
- Bookings are appended to `bookings.csv`.
- Seat state is persisted in `seats.dat` between runs.

---

## Windows notes (PowerShell)

1. Open PowerShell and `cd` into the project folder.

2. Install Streamlit:
```powershell
python -m pip install streamlit
```

3. Compile with g++ (if using MinGW):
```powershell
g++ reservation.cpp -o reservation.exe
```
- If using `reservation.exe` the Streamlit `app.py` calls `./reservation` — modify the call in `app.py` to use `reservation.exe` if needed, or create a small wrapper script.

4. Run Streamlit:
```powershell
streamlit run app.py
```

---

## Troubleshooting

- **No response from C++ binary**: Ensure `reservation` is executable and in the same folder as `app.py`. On Linux/macOS: `chmod +x reservation`.
- **Permission denied on run**: `chmod +x reservation`
- **Python JSON errors / Unexpected output**: The C++ binary prints JSON. Do not print extra debug text to stdout.
- **Bookings with spaces in name**: The Streamlit app replaces spaces in the name when calling C++. If you need full names with spaces retained in the CSV, modify `app.py` to pass quoted arguments or use a different IPC mechanism.

---

## Files the program will create on first run
- `seats.dat` — seat occupancy (comma-separated 0/1 values for 20 seats)
- `bookings.csv` — appended booking lines: `bookingId,name,seat,date`

---

## Optional enhancements (you can add later)
- Use PyBind11 to import C++ classes into Python directly (no subprocess).
- Add Excel export using `openpyxl`.
- Generate PDF tickets using `reportlab`.

---

If you want, I can:
- Produce a Windows-friendly batch file and a Linux `run.sh` to automate compile + run.
- Convert C++ into header/source OOP files and add a Makefile.
- Bundle everything into a single runnable script.

Happy to package those too — tell me what you'd like next.
