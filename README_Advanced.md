ProBus — Advanced Bus Reservation (file-based)

Files:
- reservation.cpp  (C++ backend)
- app.py           (Streamlit frontend)
- style.css
- run.sh (Linux/Mac)
- run.bat (Windows)

Requirements:
- Python 3.8+ (3.11.8 works fine)
- g++ compiler
- pip, streamlit

Setup & Run:
1. In terminal, cd to project folder.
2. Compile backend:
   Linux/macOS:
     g++ reservation.cpp -o reservation -std=c++17
     chmod +x reservation
   Windows (MinGW):
     g++ reservation.cpp -o reservation.exe -std=c++17

3. Install streamlit:
   pip install streamlit

4. Run:
   streamlit run app.py

Notes:
- Buses are stored in 'buses.json'.
- Seat maps per bus are saved as 'seats_<busid>.dat'.
- Bookings stored in 'bookings.csv' with header row.
- Admin operations available via UI or CLI:
   ./reservation admin_addbus <number> <from> <to> <date> <time> <type> <seats> <fare>
   ./reservation admin_removebus <busid>
   ./reservation admin_updatebus <busid> <field> <value>

Examples:
- Add a sample bus via CLI:
  ./reservation admin_addbus BUS100 "CityA" "CityB" 2025-12-15 09:00 AC 20 299.50

- Search:
  ./reservation search CityA CityB 2025-12-15

- Show seats:
  ./reservation seats <busid>

- Book a seat:
  ./reservation book <busid> 5 Alice 9876543210

- Cancel by PNR:
  ./reservation cancel PNR123456

If you want I can:
- Convert backend into multiple header/source files.
- Add validation (phone, email), seat layout mapping (2x2, 3x2), fare calculation by distance.
- Provide sample seed data (a few buses pre-added).
- Create a packaged zip with everything ready to run.

Enjoy!
