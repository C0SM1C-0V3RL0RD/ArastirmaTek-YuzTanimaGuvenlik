import serial
import sqlite3
from datetime import datetime
import pygame
import time

pygame.mixer.init()

ser = serial.Serial('COM4', 9600)

conn = sqlite3.connect('giris_kaydi.db')
cursor = conn.cursor()


cursor.execute('''
    CREATE TABLE IF NOT EXISTS kayitlar (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        tarih TEXT,
        saat TEXT,
        ad TEXT,
        soyad TEXT,
        durum TEXT
    )
''')
conn.commit()

print("Dinleniyor...")

def oynat_ses(dosya):
    try:
        pygame.mixer.music.load(dosya)
        pygame.mixer.music.play()
        while pygame.mixer.music.get_busy():
            time.sleep(0.1)  
    except Exception as e:
        print("Ses dosyası hatası:", e)

while True:
    try:
        line = ser.readline().decode('latin-1').strip()
        print("Gelen veri:", line)

        parts = line.split(",")
        if len(parts) != 3:
            print("Hatalı format:", parts)
            continue

        durum, ad, soyad = parts
        now = datetime.now()
        tarih = now.strftime("%Y-%m-%d")
        saat = now.strftime("%H:%M:%S")

        cursor.execute("INSERT INTO kayitlar (tarih, saat, ad, soyad, durum) VALUES (?, ?, ?, ?, ?)",
                       (tarih, saat, ad, soyad, durum))
        conn.commit()
        print("durum:", durum)

        if durum == "GIRIS":
            oynat_ses("welcome.mp3")
        elif durum == "RED":
            oynat_ses("NotWelcome.mp3")

    except Exception as e:
        print("Hata:", e)
