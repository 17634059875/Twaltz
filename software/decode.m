rf_freq = 433e6;    % carrier frequency, used to correct clock drift
sf = 12;             % spreading factor
bw = 500e3;         % bandwidth
fs = 0.5e6;           % sampling rate
chriplen=pow2(sf);

%%
%upchrip   downchrip
upchrip=LoRaPHY.chirp(true, sf, bw, fs, 0, 0, 0);
downchrip=LoRaPHY.chirp(false, sf, bw, fs, 0, 0, 0);

% LoRaPHY.psp(upchrip, fs);
% LoRaPHY.psp(downchrip, fs);

% sig1=upchrip;
% for a = 1:50
% sig1=[sig1;upchrip];
% end

% sig1=[upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip;upchrip];

% LoRaPHY.spec(sig1, fs, bw, sf);
% LoRaPHY.psp(sig1, fs);

% LoRaPHY.write(sig1, './sig.cfile');
LoRaPHY.write(upchrip, './sig.cfile');

%%
% load complex signal from file
usrp = LoRaPHY.read("D:\wendang\MATLAB\LoRaDecoding\RXsf7lora");
% usrp = LoRaPHY.read("C:\Users\Public\rx1");

index1=30000+chriplen*4;
sig = usrp(index1:index1+chriplen*5);
LoRaPHY.psp(sig, fs);

index1=30000;
L=1000;  decodebit=zeros(L,1);

for ii = 1:L
sig = usrp(index1+ii*chriplen:index1+ii*chriplen+chriplen-1);
% LoRaPHY.psp(sig, fs);
ft = fft(sig.*downchrip , chriplen);
ft_ = abs(ft(1:chriplen/2)) + abs(ft(chriplen-chriplen/2+1:chriplen));
% figure;
% t=1:chriplen;
% stem(t,abs(ft),'color','black','linewidth',0.5);
[pk1,locs1]=findpeaks(abs(ft),'MinPeakHeight',10,'MinPeakDistance',10);
% figure;
% stem(locs1, pk1,'color','black','linewidth',0.5);
if (length(locs1)>1)
decodebit(ii+1) = 1;
else
decodebit(ii+1) = 0;
end
end
disp(decodebit);
endindex1=index1+ii*chriplen+chriplen-1;

%%
codebit=[1;1;0;1;0;1;1;1;0;0;1;1;1;1;0;0];
% codebit=[1;0;1;0;1;0;1;0;1;0;1;0;1;0;1;0];
% codebit=[1;1;1;1;1;1;1;1;1;1;1;1;1;1;1;1];
index2 = 5;
endindex2 = length(decodebit)-rem(length(decodebit)-index2+1,16);
decodebit1=decodebit(index2:endindex2);
% bit=zeros(length(decodebit1),1);
num1=(length(decodebit1))/16;
bit=codebit;
for m = 1:(num1-1)
bit=[bit;codebit];
end
% ber1 = biterr (decodebit1,bit);
% BER = ber1/length(bit)




