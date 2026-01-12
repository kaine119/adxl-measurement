import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from math import sqrt

df = pd.read_csv("postprocess/results/test5.csv")[4211:7939]
vecs = df[['x', 'y', 'z']].to_numpy()
mags = np.linalg.norm(vecs, axis=1)

print(mags - np.average(mags))
fft = np.fft.fft(mags - np.average(mags))

plt.figure()
print(max(df['timestamp']))
plt.plot(df['timestamp'], mags)
plt.plot(df['timestamp'], mags - np.average(mags))
plt.show()

plt.figure()
print(fft)
plt.plot(fft)
plt.show()