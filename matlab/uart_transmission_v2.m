clc
close all
clear all
global dat
delete(instrfind);
dat = serial('COM5', 'BaudRate', 19200);
dat.InputBufferSize = 4096;
set(gcf,'CurrentCharacter','!','WindowStyle','modal');
fopen(dat);
set(dat, 'ByteOrder', 'littleEndian'); 
disp 'Connection is ready!'
figure(1)
pause(10/1000)
while(1)

    key=get(gcf,'CurrentCharacter');
    if(key=='s')
        
        fwrite(dat, 's', 'uint8'); 
        colour_defined=fread(dat, 1, 'uint8');%1 - yes;0 - no 
        if(colour_defined==1)
            colour_code=fread(dat, 1, 'uint8')
        elseif (colour_defined==0)
            disp 'Colour has not defined.'
        end
        
    elseif (key=='f')
        close all
        break;
    end
    set(gcf,'CurrentCharacter','!');
  pause(1/1000000)
end

fclose(dat);
disp 'Connection is closed!'
