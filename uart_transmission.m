clc
close all
clear all
global dat
delete(instrfind);
dat = serial('COM5', 'BaudRate', 9600);
dat.InputBufferSize = 4096;

fopen(dat); 
set(dat, 'ByteOrder', 'littleEndian'); 
disp 'Connection is ready!'
total=[];
for i=1:1000
    red = fread(dat,2,'uint8');
    red = typecast(uint8([red(1), red(2)]), 'uint16');
    green = fread(dat,2,'uint8');
    green = typecast(uint8([green(1), green(2)]), 'uint16');
    blue = fread(dat,2,'uint8');
    blue = typecast(uint8([blue(1), blue(2)]), 'uint16');
    cur=[red green blue];
    total=[total;cur]
end
fclose(dat);

disp 'Connection is closed!'
