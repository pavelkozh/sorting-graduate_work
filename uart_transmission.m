clc
close all
clear all
global dat
delete(instrfind);
dat = serial('COM5', 'BaudRate', 9600);
dat.InputBufferSize = 4096;
set(gcf,'CurrentCharacter','!','WindowStyle','modal');
fopen(dat);
set(dat, 'ByteOrder', 'littleEndian'); 
disp 'Connection is ready!'
disp 'Press "s" to start!'
total=[];
flag=0;
indicator=[1 0 0; 0.850 0.325 0.098; 1 1 0; 0 1 0; ...
0 1 1; 0 0 1; 1 0 1];
plot(0,0,'o','MarkerSize',72,'MarkerFaceColor',[1 1 1],'MarkerEdgeColor','none')
while(1)

    key=get(gcf,'CurrentCharacter');
    if(key=='s')
        colour_defined=fread(dat, 1, 'uint8');%1 - yes;0 - no 
        if(colour_defined==1)
            flag=1;
            colour_code=fread(dat, 1, 'uint8');
            total=[total;colour_code];
            T=tabulate(total);
            [Max_element,index]=max(T(:,2));
            current_colour=T(index,1);
        elseif(colour_defined==0)
            if(flag==1)
            disp(current_colour);
            plot(0,0,'o','MarkerSize',72,'MarkerFaceColor',indicator(current_colour+1,:),...
                'MarkerEdgeColor','none')
            total=0;
            flag=0;
            end
        end
        
    elseif (key=='f')
        close all
        break;
    end
  pause(1/1000000)
end

fclose(dat);
disp 'Connection is closed!'
