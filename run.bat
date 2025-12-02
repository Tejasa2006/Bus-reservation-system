@echo off
g++ reservation.cpp -o reservation.exe -std=c++17
pip install --upgrade streamlit
echo Starting Streamlit...
streamlit run app.py
pause
