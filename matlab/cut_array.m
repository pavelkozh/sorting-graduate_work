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
total=[];
while(1)
    fwrite(dat,'s','uint8');
    array_size_cut = fread(dat,1,'uint16');
    %array_size_cut = typecast(uint8([array_size_cut(2), array_size_cut(1)]), 'uint16');
    for i=1:array_size_cut
        cur=fread(dat,1,'uint8');
        total=[total cur];
    end
    total
    total=[];
end

fclose(dat);
disp 'Connection is closed!'