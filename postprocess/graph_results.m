data = readtable(['results/actual_high_2.csv']);
timestamps = data{:, 1} / 1e6; % Convert timestamps from microseconds to seconds
vectors = [data{:, 2}, data{:, 3}, data{:, 4}];

% Crop timestamps and vectors to the range t=2 to t=10
time_range = timestamps >= 9.5 & timestamps <= 14; % Logical index for the desired time range
timestamps = timestamps(time_range); % Crop timestamps
vectors = vectors(time_range, :); % Crop vectors corresponding to the cropped timestamps

magnitudes = sqrt(sum(vectors.^2, 2)); % Calculate the magnitude of each vector
% Normalize magnitudes around 0
magnitudes = magnitudes - mean(magnitudes); % Subtract the mean to center around 0

% Plot magnitudes against timestamps
figure;
plot(timestamps, magnitudes);
xlabel('Time (s)');
ylabel('Magnitude');
title('Magnitude vs Time');
grid on;

% Perform FFT on the magnitudes
Fs = 1 / mean(diff(timestamps)); % Sampling frequency
L = length(magnitudes); % Length of the signal
Y = fft(magnitudes); % Compute the FFT
P2 = abs(Y / L); % Two-sided spectrum
P1 = P2(1:L/2+1); % Single-sided spectrum
P1(2:end-1) = 2 * P1(2:end-1); % Correct the amplitude

f = Fs * (0:(L/2)) / L; % Frequency domain

% Display the sampling frequency
disp(['The sampling frequency is: ', num2str(Fs), ' Hz']);

% Calculate the Nyquist frequency
nyquist_frequency = Fs / 2; % Nyquist frequency is half the sampling frequency
disp(['The largest frequency that can be reliably detected is: ', num2str(nyquist_frequency), ' Hz']);

% Plot the single-sided amplitude spectrum
figure;
plot(f, P1);
xlabel('Frequency (Hz)');
ylabel('Magnitude');
title('Single-Sided Amplitude Spectrum of Magnitudes');
grid on;

% Find the index of the maximum non-zero value in the single-sided spectrum
non_zero_indices = find(P1 > 0); % Indices of non-zero values
[~, maxIndex] = max(P1(non_zero_indices)); % Find the maximum among non-zero values
dominant_frequency = f(non_zero_indices(maxIndex)); % Get the corresponding frequency

% Display the dominant frequency
disp(['The dominant non-zero frequency is: ', num2str(dominant_frequency), ' Hz']);