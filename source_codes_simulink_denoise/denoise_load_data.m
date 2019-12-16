%LOADS clean_audio variable for file 'denoise_simulink'
%RUN This script before running denoise_simulink
clearvars;
[y,fs]=audioread("clean_audio.wav");
clean_audio=struct();
clean_audio.signals.values=y;
clean_audio.signals.dimensions=1;
clean_audio.time=[];
