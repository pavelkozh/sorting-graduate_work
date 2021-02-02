clc
close all
clear all
global dat
delete(instrfind);
dat = serial('COM5', 'BaudRate', 19200);
dat.InputBufferSize = 4096;
fopen(dat);
set(dat, 'ByteOrder', 'littleEndian'); 
disp 'Connection is ready!'
disp 'Press "s" to start!'
indicator=[1 0 0; 0.850 0.325 0.098; 1 1 0; 0 1 0; ...
0 1 1; 0 0 1; 1 0 1];
total=[];

    r=fread(dat,1,'uint16');
    g=fread(dat,1,'uint16');
    b=fread(dat,1,'uint16');
    h=fread(dat,1,'single');
    s=fread(dat,1,'single');
    v=fread(dat,1,'single');
    rgb_res=[r g b]
    hsv_res=[h s v]


fclose(dat);
disp 'Connection is closed!'