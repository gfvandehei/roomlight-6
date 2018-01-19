import pyaudio
import numpy as np
import threading
import serial
from tkinter import *
import serial
import pyowm


class GUI(Frame):
    def audio_thread(self):
        '''
        This function is in charge of collecting data from microphone
        and changing it into usable data. Is meant to be ran as a thread
        so it does not obstruct the GUI
        '''
        self.audio_exit=False
        CHUNK = 2**11
        RATE = 44100
        p=pyaudio.PyAudio()
        stream=p.open(format=pyaudio.paInt16,channels=1,rate=RATE,input=True, frames_per_buffer=CHUNK)

        while(self.audio_exit==False):
            data = np.fromstring(stream.read(CHUNK),dtype=np.int16)
            peak=np.average(np.abs(data))*2
            bars="#"*int(50*peak/2**16)
            henlo=int(50*peak/2**14)
            henlo2=str(henlo)

            try:
                self.ser.write(bytes(henlo2+'\n',"utf-8"))
            except:
                print("NUT")
            
        stream.stop_stream()
        stream.close()
        p.terminate()

    def audio_original(self):
        '''
        this function is in charge of controlling the audio visualizer thread
        should be called by clicking the audio visualizer button
        '''
        if(self.connect_Status==False):
            print("Arduino must be connected to use audio visualizer")
        else:
            self.audio_exit=not self.audio_exit
            self.ser.write(bytes('a',"utf-8"))
            self.visualthread=threading.Thread(target=self.audio_thread)
            self.visualthread.start()

    def audio_center(self):
        if(self.connect_Status==False):
            print("Arduino must be connected to use audio visualizer")
        else:
            self.audio_exit=not self.audio_exit
            self.ser.write(bytes('b',"utf-8"))
            self.visualthread=threading.Thread(target=self.audio_thread)
            self.visualthread.start()

    def weather_command(self):
        if(self.connect_Status==False):
            print("Arduino must be connected to use weather function")
        else:
            self.ser.write(bytes('c',"utf-8"))
            w=self.forcast.get_weather()
            status=w.get_status()
            #clear=1, clouds=2, Snow=3, rain=4, thunderstorm=5, Drizzle=6, extreme=7
            if(status=="Clear"):
                self.ser.write(bytes(str(1)+'\n',"utf-8"))
            if(status=="Clouds"):
                self.ser.write(bytes(str(2)+'\n',"utf-8"))
            if(status=="Snow"):
                self.ser.write(bytes(str(3)+'\n',"utf-8"))
            if(status=="Rain"):
                self.ser.write(bytes(str(4)+'\n',"utf-8"))
            if(status=="Thunderstorm"):
                self.ser.write(bytes(str(5)+'\n',"utf-8"))
            if(status=="Drizzle"):
                self.ser.write(bytes(str(6)+'\n',"utf-8"))
            if(status=="Extreme"):
                self.ser.write(bytes(str(7)+'\n',"utf-8"))

    def audio_rainbow(self):
        if(self.connect_Status==False):
            print("Arduino must be connected to use audio visualizer")
        else:
            self.audio_exit=not self.audio_exit
            self.ser.write(bytes('d',"utf-8"))
            self.visualthread=threading.Thread(target=self.audio_thread)
            self.visualthread.start()    

    def rainbow_command(self):
        if(self.connect_Status==False):
            print("Arduino must be connected to use lighting effects")
        else:
            self.audio_exit=not self.audio_exit
            self.ser.write(bytes('e',"utf-8"))

    def audio_sides(self):
        if(self.connect_Status==False):
            print("Arduino must be connected to use audio visualizer")
        else:
            self.audio_exit=not self.audio_exit
            self.ser.write(bytes('f',"utf-8"))
            self.visualthread=threading.Thread(target=self.audio_thread)
            self.visualthread.start()

    def reset_command(self):
        '''
        this is the function for the reset button, resets the 
        '''
        if(self.connect_Status==False):
            print("Arduino must be connected to use this button")
        else:
            self.ser.write(bytes('z',"utf-8"))
            self.audio_exit=not self.audio_exit
            print(self.audio_exit)
    
    def createWidgets(self):

        self.ModeVar=StringVar()
        self.ModeVar.set("Please select a mode")
        self.modeDisplay=Label(textvariable=self.ModeVar)
        self.right_labelframe=LabelFrame(text="Coming soon")
        self.mid_labelframe=LabelFrame(text="Display Modes")
        self.left_labelframe=LabelFrame(text="Audio Visualizers")
        self.indicatorFrame=LabelFrame(text="Indicators")
        #these are the widgets for the left labelframe
        self.av1=Button(self.left_labelframe,text="Original Light",width=10,command=self.audio_original)
        self.av2=Button(self.left_labelframe,text="Center Light",width=10,command=self.audio_center)
        self.av3=Button(self.left_labelframe,text="Edge Light",width=10,command=self.audio_sides)
        self.av4=Button(self.left_labelframe,text="Rainbow",width=10,command=self.audio_rainbow)
        #these are the widgets for the middle frame
        self.display1=Button(self.mid_labelframe,text="Display1",width=10)
        self.display2=Button(self.mid_labelframe,text="Display2",width=10)
        self.display3=Button(self.mid_labelframe,text="Display3",width=10)
        self.display4=Button(self.mid_labelframe,text="Display4",width=10)
        #these are the widgets for the right labelframe
        self.right_1=Button(self.right_labelframe,text="right_1",width=10)
        self.right_2=Button(self.right_labelframe,text="right_2",width=10)
        self.right_3=Button(self.right_labelframe,text="right_3",width=10)
        self.right_4=Button(self.right_labelframe,text="right_4",width=10)
        #these are the widgets for indicatorframe
        self.connectLight=Button(self.indicatorFrame,text="Serial",state=DISABLED,bg='red',width=5)

        #attach to GUI section
        self.modeDisplay.grid(row=0,column=1, columnspan=2)
        self.right_labelframe.grid(row=1, column=2)
        self.mid_labelframe.grid(row=1,column=1)
        self.left_labelframe.grid(row=1,column=0)
        self.indicatorFrame.grid(row=1,column=3)
        #packs left frame buttons
        self.av1.pack()
        self.av2.pack()
        self.av3.pack()
        self.av4.pack()
        #packs mid frame
        self.display1.pack()
        self.display2.pack()
        self.display3.pack()
        self.display4.pack()
        #puts right frame buttons in frame
        self.right_1.pack()
        self.right_2.pack()
        self.right_3.pack()
        self.right_4.pack()
        #packs indicatorframe widgets
        self.connectLight.pack()

        '''self.reset_button=Button(text="reset", command=self.reset_command)
        self.lightdisplay=Message(width=300,text="-"*72)
        self.redval=Spinbox(from_=0,to=255)#begin row 1
        self.greenval=Spinbox(from_=0,to=255)
        self.blueval=Spinbox(from_=0,to=255)
        self.LEDSelector=Spinbox(from_=0,to=self.LEDNUM)
        self.colorEnter=Button(text="Display")#end row 1
        self.TimerIn=Entry()#begin row 2
        self.TimerCheck=Button(text="Start")#row 3

        self.AudioVisualizer=Button(text="Audio Visualizer Side",command=self.audio_original)#row 4
        self.weather=Button(text="Weather",command=self.weather_command)
        self.AudioVisualizer2=Button(text="Audio Visualizer Center",command=self.audio_center)
        self.RainbowAudio=Button(text="Audio Visualizer Rainbow",command=self.audio_rainbow)
        self.AudioVisualizerSides=Button(text="Audio Visualizer Sides",command=self.audio_sides)
        self.Rainbow=Button(text="Rainbow",command=self.rainbow_command)
        self.screen=Button(text="TBD")
        self.TBD1=Button(text="TBD")
        self.TBD2=Button(text="TBD")

        #pack row 1
        self.lightdisplay.grid(row=0)
        self.reset_button.grid(row=1, column=0)
        self.redval.grid(row=2,column=0)
        self.greenval.grid(row=3, column=0)
        self.blueval.grid(row=0,column=1)
        self.colorEnter.grid(row=1,column=1)
        self.weather.grid(row=2,column=1)
        self.TimerIn.grid(row=0,column=2)
        self.TimerCheck.grid(row=1,column=2)
        self.AudioVisualizer.grid(row=2,column=2)
        self.Rainbow.grid(row=0,column=3)
        self.screen.grid(row=1,column=3)
        self.RainbowAudio.grid(row=2,column=3)
        self.AudioVisualizerSides.grid(row=3,column=3)
        self.TBD1.grid(row=3,column=1)
        self.AudioVisualizer2.grid(row=3,column=2)
        #end pack'''

    def __init__(self, master=None):
        #class variable definition
        self.LEDNUM=0
        self.audio_exit=False
        self.connect_Status=False #used to store if an arduino was found
        self.owm=pyowm.OWM("3bfa77875cd0a2c86b4a809e701a2833")
        self.forcast=self.owm.weather_at_coords(42.728409,-73.691788)
        #local variable definition
        comlist_windows=["COM2","COM3","COM4","COM5","COM6","COM7"]#COM1 is not included because that is reserved
        comlist_Linux=["/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyACM0","/dev/ttyACM1","/dev/ttyACM2"]
        Frame.__init__(self, master)
        self.createWidgets()

        if(sys.platform=="linux"):
            #this attempts to connect to available devices to see if an arduino is connected
            print("Attempting to connect (Linux)")
            for i in comlist_Linux:
                try:
                    #attempts to connect to serial device
                    self.ser=serial.Serial(i, 9600)
                except:
                    print("could not connect to "+i+" (Linux)")
                    continue
                self.connect=True
                break
        else:
            #this attempts to connect to available com ports to see if an arduino is connected
            print("Attempting to connect (WINDOWS)")
            for i in comlist_windows:
                try:
                    #attempts to connect to com port
                    self.ser=serial.Serial(i, 9600)
                except:
                    print("could not connect to "+i+" (Windows)")
                    continue
                self.connect_Status=True
                break

        if self.connect_Status==True:
            print("You have connected to device on "+i)
            self.connectLight["bg"]='green'
        else:
            print("There has been some trouble connecting to device, please reconnect and try again")





if __name__=="__main__":
    root=Tk()
    mainWindow=GUI(master=root)
    mainWindow.master.title("Neopixel Control Interface")
    mainWindow.master.wm_iconbitmap('laylabytes.ico')
    mainWindow.mainloop()