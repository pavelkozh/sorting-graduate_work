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
disp 'Press "s" to start!'
indicator=[1 0 0; 1 0.5 0; 1 1 0; 0 1 0; ...
0 1 1; 0 0 1; 1 0 1];
plot(0,0,'o','MarkerSize',72,'MarkerFaceColor',[1 1 1],'MarkerEdgeColor','none')

while(1)
     key = get(gcf,'CurrentCharacter');
     colour_defined = fread(dat,1,'uint8');
     if(key == 's')
         if (colour_defined==1)
            colour_code = fread(dat,1,'uint8');
            disp(colour_code);
            plot(0,0,'o','MarkerSize',72,'MarkerFaceColor',indicator(colour_code,:),...
            'MarkerEdgeColor','none')
         end
     elseif (key == 'f')
         
        close all
        break;
        
     end
  pause(1/1000000)
end

fclose(dat);
disp 'Connection is closed!'