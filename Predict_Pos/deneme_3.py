import matplotlib.pyplot as plt
import numpy as np
import ast

# Dosya yolları
file_path = "C:/Users/Halit/Desktop/pid_out.txt"
file_path_1 = "C:/Users/Halit/Desktop/real_konum.txt"

# Dosyadan verileri oku
with open(file_path, "r") as f:
    lines = f.readlines()

with open(file_path_1, "r") as f:
    lines_1 = f.readlines()

# PID tahmin verisi
data = []
for line in lines:
    try:
        value = float(line.strip())
        data.append(value)
    except ValueError:
        pass

# Gerçek konum verisi (x değerleri)
data_1 = []
for line in lines_1:
    try:
        position = ast.literal_eval(line.strip())
        if isinstance(position, list) and len(position) == 2:
            x_value = float(position[0])
            data_1.append(x_value)
    except (ValueError, SyntaxError):
        pass

# Veri kontrolü
if len(data) < 1 or len(data_1) < 2:
    print("Yeterli veri yok.")
    exit()

# Ortak uzunluk: PID tahminlerinden biri eksik olacak (kaydırılmalı)
min_len = min(len(data) - 1, len(data_1) - 1)

aligned_pid  = np.array(data[:min_len])
aligned_real = np.array(data_1[1:min_len + 1])  # gerçek konum 1'den başlıyor
x = np.arange(min_len)

# Grafik çizimi
plt.figure(figsize=(10, 6))
plt.plot(x, aligned_pid, label="Tahmin Değeri (t)", color="blue", marker='o')
plt.plot(x, aligned_real, label="Gerçek Konum (t+1)", color="orange", marker='x')
plt.title("PID Tahmini (t) ile Gerçek Konum (t+1) Zaman Kaydırmalı Karşılaştırması")
plt.xlabel("Zaman Adımı")
plt.ylabel("X Koordinatı")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
