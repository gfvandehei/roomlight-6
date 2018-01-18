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
                #print("THIS IS IT",bytes(henlo2,"utf-8"))
                self.ser.write(bytes(henlo2+'\n',"utf-8"))
                #print(self.ser.read())
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
        self.audio_exit=not self.audio_exit
        self.ser.write(bytes('a',"utf-8"))
        self.visualthread=threading.Thread(target=self.audio_thread)
        self.visualthread.start()

    def audio_center(self):
        self.audio_exit=not self.audio_exit
        self.ser.write(bytes('b',"utf-8"))
        self.visualthread=threading.Thread(target=self.audio_thread)
        self.visualthread.start()

    def weather_command(self):
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
        self.audio_exit=not self.audio_exit
        self.ser.write(bytes('d',"utf-8"))
        self.visualthread=threading.Thread(target=self.audio_thread)
        self.visualthread.start()    

    def rainbow_command(self):
        self.audio_exit=not self.audio_exit
        self.ser.write(bytes('e',"utf-8"))

    def audio_sides(self):
        self.audio_exit=not self.audio_exit
        self.ser.write(bytes('f',"utf-8"))
        self.visualthread=threading.Thread(target=self.audio_thread)
        self.visualthread.start()

    def reset_command(self):
        '''
        this is the function for the reset button, resets the 
        '''
        self.ser.write(bytes('z',"utf-8"))
        self.audio_exit=not self.audio_exit
        print(self.audio_exit)
    
    def createWidgets(self):

        self.reset_button=Button(text="reset", command=self.reset_command)
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
        #end pack

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
        
        if(sys.platform=="linux"):
            #this attempts to connect to available com ports to see if an arduino is connected
            print("Attempting to connect (Linux)")
            for i in comlist_Linux:
                try:
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
                    self.ser=serial.Serial(i, 9600)
                except:
                    print("could not connect to "+i+" (Windows)")
                    continue
                self.connect_Status=True
                break

            if self.connect_Status==True:
                print("You have connected to device on "+i)
            else:
                print("There has been some trouble connecting to device, please reconnect and try again")
        
        self.createWidgets()





if __name__=="__main__":
    root=Tk()
    mainWindow=GUI(master=root)
    mainWindow.mainloop()