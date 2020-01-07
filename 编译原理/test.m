function plot_rose
draw_main(450,90);
function draw_main(x,y)
%粉红色玫�?
arcdata{1}=[65 -60 150 350 8
    66 -54 300 470 8
    65 -56 30 230 10
    64 -57 300 490 17];
ellipsedata{1}=[73 -30 250 450 27 40
    59 -30 100 290 27 40
    65  -40 140 270 20 30];
arcdata{2}=[0 0 150 350 12
    1 8 280 470 12
    0 2 30 230 16
    0 3 80 240 28
    2 8 180 330 22
    -2 2 310 460 25];
ellipsedata{2}=[-12 30 120 300 30 40
    10 28 250 423 30 42
    -4  10 290 393 30 40];
ellipsedata{3}=[120 5 0 360 15 25];
ellipsedata{4}=[-70 10 0 360 14 20];
rose_e{2}=[x+16 y+32 235 355 26 35
    x-15 y+32 190 310 30 35
    x+0 y+35 190 350 43 50];
rose_e{1}=[x+80 y-48 220 350 22 50
    x+50 y-48 190 320 22 50
    x+65 y-28 180 360 36 50];
rose_e{3}=[x+120 y-6 200 340 17 25
    x+120 y+7 160 380 17 27];
rose_e{4}=[x-70 y+15 140 390 17 20
    x-75 y-10 205 340 10 30
    x-60 y-10 195 340 5 30];
arcdata{3}=[0 82 190 350 6];
ss={'m','r','b','y'};
s0={'k','k','g'};
figure('menubar','none','numbertitle','off','name','rose');
% figure('numbertitle','off','name','rose');
hold on
for j=1:length(ellipsedata)
    for i=1:size(ellipsedata{j},1)
        rectangle('Position',[x+ellipsedata{j}(i,1)-ellipsedata{j}(i,5),y+ellipsedata{j}(i,2)-ellipsedata{j}(i,6),2*ellipsedata{j}(i,5),2*ellipsedata{j}(i,6)],'Curvature',[1,1],...
            'FaceColor',ss{j})
    end
    if j<4
        for i=1:size(arcdata{j},1)
            draw_arc(x+arcdata{j}(i,1),y+arcdata{j}(i,2),arcdata{j}(i,3),arcdata{j}(i,4),arcdata{j}(i,5),s0{j});
        end
    end
    for i=1:size(rose_e{j},1)
        plot_rose_e(rose_e{j},j);
    end
end
for j=1:3
    zhuzhi=[x-98,y+100+j,255,371,100,80
        x-20,y+30+j,260,358,140,140
        x+224,y+20+j,180,218,160,140];
    draw_branch(zhuzhi);
end
ce_branch=[x+70,y+34,180,233,140,140; 
  x,y+40,205,255,100,120; 
  x+135,y-30,209,249,72,120; 
  x,y+20,263,301,100,120; 
  x+85,y-10,278,305,100,120; 
  x+100,y-62,282,308,90,120; 
  x-50,y-10,277,314,30,120; 
  x+70,y+80,222,266,52,120; 
  x-60,y-45,229,266,52,120; 
  x+79,y-45,229,266,52,120; 
  x+84,y,224,273,52,120; 
  x+110,y+40,240,282,100,120];
draw_branch(ce_branch);
t_leaf=[x+168,y+282,10,20,-40; 
  x+160,y+252,8,16,260; 
  x+145,y+270,8,16,-15; 
  x+156,y+224,10,20,-45; 
  x+150,y+200,8,16,270; 
  x+135,y+220,8,16,-10; 
  x+146,y+144,8,16,-80; 
  x+130,y+130,6,12,235; 
  x+125,y+154,7,14,-10; 
  x+78,y+98,6,12,-90; 
  x+60,y+90,5,10,180; 
  x+70,y+109,5,10,-45;
  x-125,y+270,12,24,60; 
  x-95,y+270,10,20,10; 
  x-110,y+245,10,20,90; 
  x-105,y+220,10,20,45; 
  x-100,y+190,8,16,135; 
  x-75,y+210,8,16,-45; 
  x+65,y+190,10,20,-45; 
  x+40,y+185,8,16,0; 
  x+55,y+165,8,16,90];
for j=1:size(t_leaf,1)
    draw_elli(t_leaf(j,1),t_leaf(j,2),t_leaf(j,3),t_leaf(j,4),t_leaf(j,5));
end
axis ij
axis off
set(gcf,'color','k');
daspect([1,1,1])

% 画旋转椭�?
function draw_elli(x0,y0,a,b,theta)
theta=-theta;
t = 0:0.01:2*pi;
x =  a*cos(t);
y =  b*sin(t);
xy = zeros(2,length(t));
for i = 1:length(t)
    xy(:,i) = expm([0,-pi/180*theta;pi/180*theta,0])*[x(i);y(i)];
end
plot(x0 +xy(1,:),480-y0 + xy(2,:),'g');
% 画弧�?
function draw_arc(x0,y0,st_a,en_a,r,c_c)
t = (pi/180*st_a:0.01:pi/180*en_a)-pi;
x = x0 + r*cos(t);
y = y0 + r*sin(t);
plot(x,y,c_c);
% 花萼
function plot_rose_e(matr,ch)
x=[];y=[];n=size(matr,1);
for i=1:size(matr,1)
    t=(matr(i,3):matr(i,4))*pi/180-pi;
    x1=matr(i,1)+matr(i,5)*cos(t);
    y1=matr(i,2)+matr(i,6)*sin(t);
    if i<3
        x1=2*matr(i,1)-x1;
    end
    x1=fliplr(x1);
    y1=fliplr(y1);
    if n<3 && i<2
        x1=fliplr(x1);
        y1=fliplr(y1);
    end
    if ch==4 && i==2
        x1=fliplr(x1);
        y1=fliplr(y1);
    end
    
    x=[x,x1];
    y=[y,y1];
end
fill(x,y,'g');
% 画树�?
function draw_branch(zhuzhi)
for k=1:size(zhuzhi,1)
    t=(zhuzhi(k,3):zhuzhi(k,4))*pi/180-pi;
    x1=zhuzhi(k,1)+zhuzhi(k,5)*cos(t);
    y1=zhuzhi(k,2)+zhuzhi(k,6)*sin(t);
    x1=2*zhuzhi(k,1)-x1;
    plot(x1,y1,'g','linewidth',2);
end