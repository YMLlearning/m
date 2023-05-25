import sensor, image, time,lcd
from pyb import UART


threshold_1=(71, 255)
threshold = (0, 70)#车1 50
threshold_2=(0, 28, -50, 69, -68, 87)
black_threshold = (25, 255)
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)   #选择灰度模式
sensor.set_framesize(sensor.QQVGA)       #160*120选择分辨率（分辨率越高帧率可能越）
sensor.skip_frames(time = 2000)

#uart_A = UART(UART.UART1, 115200, 8, 0, 0, timeout=1000, read_buf_len=4096)
lcd.init()
sensor.set_hmirror(True)
uart = UART(3, 115200)
data4=9
line_roi1 = (0,100,160,10)
line_roi2 = (0,50,160,10)
line_roi3 = (0,20,160,10)
flag=0 #0正常巡线，1正常停车，2岔路口in，3岔路口out，4非正常停车,5转弯
xx1=0
xx2=0
# 通信协议
def Send_data_packet(data1,data2,data3):#data1 = xx1,data2 = xx2,data3 = flag
    x = '%03d'%data1
    y = '%03d'%data2
    z = '%01d'%data3
    w = '%01d'%data4
    data = 'W'+x+y+z+w+'Z'
    uart.write(data)

#求绝对值
def my_abs(x,y):
    xx = x - y
    if(xx>0):
        return xx
    else:
        return -xx

def run():
     global flag #0正常巡线，1正常停车，2岔路口in，3岔路口out，4非正常停车,5转弯
     global xx1
     global xx2
     blobs_low = img.find_blobs([black_threshold], roi = line_roi1, pixels_threshold = 50)
     blobs_high = img.find_blobs([black_threshold], roi = line_roi2, pixels_threshold = 50)
     blobs_higher = img.find_blobs([black_threshold], roi = line_roi3, pixels_threshold = 50)

     if(blobs_low and blobs_high):
         if(blobs_high[0].w()<=55 and len(blobs_high)==1 and blobs_high[0].h()*blobs_high[0].w()>=480 and my_abs(blobs_high[0].cx(),blobs_low[0].cx())<20 ):#停车0.3m下
            print("stop")
            flag = 1
            xx1 = 80
            xx2 = 0
         elif(len(blobs_high)==1 and len(blobs_low)==1):#正常巡线
            len_h_l=my_abs(blobs_high[0].cx(),blobs_low[0].cx())
            if(len_h_l<5):
                print("line")
                flag = 0
                img.draw_cross(blobs_high[0].cx(), blobs_high[0].cy(), (255,255,255), 30)
                xx1 = blobs_high[0].cx()
                xx2 = 0
                print(xx1)
            else:
                print("line_wan")
                flag = 5
                img.draw_cross(blobs_high[0].cx(), blobs_high[0].cy(), (255,255,255), 30)
                xx1 = blobs_high[0].cx()
                xx2 = 0
                print(xx1)
         elif(len(blobs_high)==2 and len(blobs_low)==2):#一类出入三岔
            len_high = my_abs(blobs_high[0].cx(),blobs_high[1].cx())
            len_low = my_abs(blobs_low[0].cx(),blobs_low[1].cx())
            if(len_high>len_low):
                print("in")
                flag = 2
                img.draw_cross(blobs_high[0].cx(), blobs_high[0].cy(), (255,255,255), 30)
                img.draw_cross(blobs_high[1].cx(), blobs_high[1].cy(), (255,255,255), 30)
                xx1 = blobs_high[0].cx()
                xx2 = blobs_high[1].cx()
            else:
                print("out")
                flag = 3
                img.draw_cross(blobs_high[0].cx(), blobs_high[0].cy(), (255,255,255), 30)
                img.draw_cross(blobs_high[1].cx(), blobs_high[1].cy(), (255,255,255), 30)
                xx1 = blobs_high[0].cx()
                xx2 = blobs_high[1].cx()
         elif(len(blobs_high)==2 and len(blobs_higher)==2):#另一类出入三岔
            len_high = my_abs(blobs_high[0].cx(),blobs_high[1].cx())
            len_higher = my_abs(blobs_higher[0].cx(),blobs_higher[1].cx())
            if(len_high<len_higher):
                print("in")
                flag = 2
                img.draw_cross(blobs_high[0].cx(), blobs_high[0].cy(), (255,255,255), 30)
                img.draw_cross(blobs_high[1].cx(), blobs_high[1].cy(), (255,255,255), 30)
                xx1 = blobs_high[0].cx()
                xx2 = blobs_high[1].cx()
            else:
                print("out")
                flag = 3
                img.draw_cross(blobs_high[0].cx(), blobs_high[0].cy(), (255,255,255), 30)
                img.draw_cross(blobs_high[1].cx(), blobs_high[1].cy(), (255,255,255), 30)
                xx1 = blobs_high[0].cx()
                xx2 = blobs_high[1].cx()
         #elif(len(blobs_high)==2 and blobs_right ):#出弯
           # print("out")
          #  flag = 3
           # img.draw_cross(blobs_high[0].cx(), blobs_high[0].cy(), (255,255,255), 30)
           # img.draw_cross(blobs_high[1].cx(), blobs_high[1].cy(), (255,255,255), 30)
           # xx1 = blobs_high[0].cx()
           # xx2 = blobs_high[1].cx()
        # elif(len(blobs_high)==2 and blobs_left):#进弯
         #   print("in")
         #   flag = 2
         #   img.draw_cross(blobs_high[0].cx(), blobs_high[0].cy(), (255,255,255), 30)
          #  img.draw_cross(blobs_high[1].cx(), blobs_high[1].cy(), (255,255,255), 30)
          #  xx1 = blobs_high[0].cx()
          #  xx2 = blobs_high[1].cx()
     Send_data_packet(xx1,xx2,flag)
     #print(blobs_high[0].h()*blobs_high[0].w())
    # print(blobs_high[0].w())

while (True):
    img = sensor.snapshot()
    img_black=img.binary([threshold])
    #img_black.mean(2)
    run()
    img.draw_rectangle(line_roi1)
    img.draw_rectangle(line_roi2)
    img.draw_rectangle(line_roi3)

    lcd.display(img)
