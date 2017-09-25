import pyaudio
import numpy as np
import threading
from tkinter import *


class GUI(Frame):
    def audio_thread(self):
        CHUNK = 2**11
        RATE = 44100

        p=pyaudio.PyAudio()
        stream=p.open(format=pyaudio.paInt16,channels=1,rate=RATE,input=True,
                    frames_per_buffer=CHUNK)

        while(self.audio_exit==False): #go for a few seconds
            data = np.fromstring(stream.read(CHUNK),dtype=np.int16)
            peak=np.average(np.abs(data))*2
            bars="#"*int(50*peak/2**16)
            print("%05d %s"%(peak,bars))

        stream.stop_stream()
        stream.close()
        p.terminate()
    def audio_command(self):
        self.visualthread=threading.Thread(target=self.audio_thread)
        self.visualthread.start()
    def reset_command(self):
        self.audio_exit=not self.audio_exit
        print(self.audio_exit)
        #self.audio_exit=True
    
    def createWidgets(self):
        self.audioExit=False
        self.lightdisplay=Canvas(height=30, width=300)
        self.redval=Spinbox(from_=0,to=255)#begin row 1
        self.greenval=Spinbox(from_=0,to=255)
        self.blueval=Spinbox(from_=0,to=255)
        self.LEDSelector=Spinbox(from_=0,to=self.LEDNUM)
        self.colorEnter=Button(text="Display")#end row 1
        self.TimerIn=Entry()#begin row 2
        self.TimerCheck=Button(text="Start")#row 3
        self.AudioVisualizer=Button(text="Audio Visualizer",command=self.audio_command)#row 4
        self.weather=Button(text="Weather",command=self.reset_command)
        self.screen=Button(text="Audio Visualizer")
        self.TBD1=Button(text="Audio Visualizer")
        self.TBD2=Button(text="Audio Visualizer")

        #pack row 1
        self.lightdisplay.grid(row=0)
        self.redval.grid(row=1,column=0)
        self.greenval.grid(row=2, column=0)
        self.blueval.grid(row=3,column=0)
        self.colorEnter.grid(row=4,column=0)
        self.TimerIn.grid(row=1,column=1)
        self.TimerCheck.grid(row=1,column=2)
        self.AudioVisualizer.grid(row=1,column=3)
        self.weather.grid(row=1,column=3)
        self.weather.grid(row=2,column=3)
        self.screen.grid(row=3,column=3)
        self.TBD1.grid(row=4,column=3)
        self.TBD2.grid(row=5,column=3)
        #end pack

    def __init__(self, master=None):
        self.audio_exit=False
        Frame.__init__(self, master)
        self.LEDNUM=0
        self.createWidgets()





if __name__=="__main__":
    root=Tk()
    mainWindow=GUI(master=root)
    mainWindow.mainloop()
    root.destroy()