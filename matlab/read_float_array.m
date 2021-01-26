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
array=[];

fwrite(dat,'s','uint8');
array_size = fread(dat,1,'uint16');
for i=1:array_size        
      current=fread(dat,1,'single');
      array=[array current];
         
end
array
%array=[];

fclose(dat);
disp 'Connection is closed!'