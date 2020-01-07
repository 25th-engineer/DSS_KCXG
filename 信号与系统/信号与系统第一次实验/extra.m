t=0:0.01:10;
f1=11*sin(6*t+pi/5);
f2=5*cos(9*t+pi/2);


plot(t,f1,'b');hold on;
plot(t,f2,'g');hold on;
plot(t,f1.*f2,'r');legend('f1','f2','f1.*f2'),hold on;


figure,plot(t,f1,'b');hold on;
plot(t,f2,'g');hold on;
plot(t,f1+f2,'r');legend('f1','f2','f1+f2'),hold off;