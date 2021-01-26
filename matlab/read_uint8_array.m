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
array_size = fread(dat,2,'uint8');
array_size = typecast(uint8([array_size(2), array_size(1)]), 'uint16');
for i=1:array_size        
      current=fread(dat,1,'uint8');
      total=[total current]
         
end
total=[];
end
fclose(dat);
disp 'Connection is closed!'