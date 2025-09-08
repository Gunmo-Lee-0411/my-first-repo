clc
clear

%% step 1

w = -5:0.01:5;

f1 = sqrt(128/pi/3).*w.^2./(1+w.^2).^(5/2);
f2 = sqrt(128/pi/5).*w./(1+w.^2).^(5/2);
f3 = sqrt(24/pi/5).*( (1./(1+w.^2)).^(5/2) - (5/3) .* (w.^2./(1+w.^2).^(5/2)) );

figure(1)
subplot(4,1,1)
plot(w,f1,w,f2,w,f3)
title("step 1")
legend()

%% step 2
subplot(4,1,2)

tt = -15:0.1:15;

pos = 0;
sig = sqrt(128/pi/5).*(2*tt-pos)./(1+(2*tt-pos).^2).^(5/2);

sig2 = sig+5*abs(sawtooth(tt/1.5-2));

plot(tt,sig2)

%% step 3

subplot(4,1,3)

scaled = 0.001;

for i = 1:length(tt)
    pos = tt(i);
    ww = (tt-pos)./scaled;
    f2 = sqrt(128/pi/5).*ww./(1+ww.^2).^(5/2);
    f1 = sqrt(128/pi/3).*ww.^2./(1+ww.^2).^(5/2);
    f3 = sqrt(24/pi/5).*( (1./(1+ww.^2)).^(5/2) - (5/3) .* (ww.^2./(1+ww.^2).^(5/2)) );
    f_sig1(i) = sig*f1';
    f_sig2(i) = sig*f2';
    f_sig3(i) = sig*f3';
    f_sig1(i) = 0;
    f_sig3(i) = 0;
    f_sig(i) = sqrt(f_sig1(i).^2+f_sig2(i).^2+f_sig3(i).^2);

    
    

end



subplot(4,1,4)
plot(tt,f_sig)