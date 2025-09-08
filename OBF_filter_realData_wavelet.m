clc


%% Processing Real Data
%% Load Data
temp_Lati_cutted=Lati_cutted;
temp_Longi_cutted=Longi_cutted;

minLati=min(Lati_linspace);
maxLati=max(Lati_linspace);
minLongi=min(Longi_linspace);
maxLongi=max(Longi_linspace);

j=1;
for i=1:length(M_cutted)
    if (minLati<Lati_cutted(i))&&(Lati_cutted(i)<maxLati)
        if(minLongi<Longi_cutted(i))&&(Longi_cutted(i)<maxLongi)
            pLati(j,1)=Lati_cutted(i);
            pLongi(j,1)=Longi_cutted(i);
            pMag(j,1)=M_cutted(i);
            j=j+1;
        end
    end
end

%% Data Sampling

SSSS=10; % 1 is original

j=1;
if SSSS==1
else
    for i=1:length(pMag)
        if rem(i,SSSS)==1
            spLati(j,1)=pLati(i);
            spLongi(j,1)=pLongi(i);
            spMag(j,1)=pMag(i);
            j=j+1;
        end
    end
    pLati=spLati;
    pLongi=spLongi;
    pMag=spMag;
end


%% plot mag data
figure(10)
subplot(4,1,2)
plot(pMag-mean(pMag))

%% Convert to meter

alt = 43;
x=(max(pLati)-min(pLati))/2+min(pLati);
y=(max(pLongi)-min(pLongi))/2+min(pLongi);
org = [x, y, alt];
[pLongi, pLati, z] = latlon2local(pLati, pLongi, alt, org);


%% pre-Filter

% pMag=smoothdata(pMag,'gaussian',20);
temp_pMag=pMag;

pMag=pMag-smoothdata(pMag,'movmean',100);

hold on
plot(pMag)

%% Plot the orthonormal basis
w=-5:0.1:5;
pos=0;
scaled=1;

f1=sqrt(128/pi/3).*((w-pos)./scaled').^2./(1+((w-pos)./scaled').^2).^(5/2);
f2=sqrt(128/pi/5).*(w-pos)./scaled./(1+((w-pos)./scaled).^2).^(5/2);
f3=sqrt(24/pi/5).*( ( 1./(1+((w-pos)./scaled').^2).^(5/2) ) - (5/3).* ( ((w-pos)./scaled').^2./(1+((w-pos)./scaled').^2).^(5/2) )   );


subplot(4,1,1)
plot(w,f1,w,f2,w,f3)
title("The orthonormal basis f1(w), f2(w), f3(w)")


%% Calculate default values for scalling

measureLength=0;
for i=2:length(pLati)
    measureLength=measureLength + sqrt(  (pLati(i)-pLati(i-1))^2   +    (  pLongi(i)-pLongi(i-1) )^2  );
end
% scale array
sc=linspace(1,100,100);
%NOTE: Orthogonal Base 위아래 피크 대강 스케일링하는 부분

scaled=sc;

%% Filtering show

tt_init=linspace(1,measureLength,length(pMag));


pos=measureLength/2;
ss=scaled(10);


f2_init=100./ss'.*sqrt(128/pi/5).*((tt_init-pos)./ss')./(1+((tt_init-pos)./ss').^2).^(5/2);


subplot(4,1,3)
plot(tt_init,f2_init,tt_init,pMag)


%% Zero padding first and last



%% Filtering, f2 only(current)
% make window
tic;

tt=linspace(1,measureLength,length(pMag));
clear f_sig
for i=1:length(tt)
    pos=tt(i);
    f1=0.1./scaled'.*sqrt(128/pi/3).*((tt-pos)./scaled').^2./(1+((tt-pos)./scaled').^2).^(5/2);
    f2=0.1./scaled'.*sqrt(128/pi/5).*(tt-pos)./scaled'./(1+((tt-pos)./scaled').^2).^(5/2);
    f3=0.1./scaled'.*sqrt(24/pi/5).*( ( 1./(1+((tt-pos)./scaled').^2).^(5/2) ) - (5/3).* ( ((tt-pos)./scaled').^2./(1+((tt-pos)./scaled').^2).^(5/2) )   );
    
%     f_sig(i,:)=(pMag'*f2').^2;

    ccc=(pMag'*f3').^2;
    aaa=(pMag'*f1').^2;
    bbb=(pMag'*f2').^2;

    ccc=0;
    aaa=0;

    f_sig(i,:)=sqrt(aaa+bbb+ccc);

    if(rem(i,100)==0)
        disp(i+"/"+length(tt))
    end
%     subplot(4,1,3)
%     plot(tt,f2,tt,pMag)
%     plot(tt,f2)
%     pause(0.1)
end

toc
%% cutting 
% settup
per_cut=0.1; % percentage

%
org_leng=length(f_sig);
cut_Num=org_leng*per_cut;
f_sig=f_sig(cut_Num:org_leng-cut_Num,:);
pLati=pLati(cut_Num:org_leng-cut_Num);
pLongi=pLongi(cut_Num:org_leng-cut_Num);
tt=tt(cut_Num:org_leng-cut_Num);

subplot(4,1,4)
plot(tt,f_sig)

figure(30)
mesh(f_sig)

%% output
f_Mag=f_sig(:,1);
f_Lati=pLati;
f_Longi=pLongi;

