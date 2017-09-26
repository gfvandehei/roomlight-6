import pyaudio
import numpy as np
import threading
import serial
from tkinter import *
import serial


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
        stream=p.open(format=pyaudio.paInt16,channels=1,rate=RATE,input=True,
                    frames_per_buffer=CHUNK)

        while(self.audio_exit==False):
            data = np.fromstring(stream.read(CHUNK),dtype=np.int16)
            peak=np.average(np.abs(data))*2
            bars="#"*int(50*peak/2**16)
            try:
                self.ser.write(50*peak/2**16)
            except:
                print()
            print(bars)

        stream.stop_stream()
        stream.close()
        p.terminate()

    def audio_command(self):
        '''
        this function is in charge of controling the audio visualizer thread
        should be called by clicking the audio visualizer button
        '''
        self.visualthread=threading.Thread(target=self.audio_thread)
        self.visualthread.start()

    def reset_command(self):
        '''
        this is the function for the reset button, resets the 
        '''
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
        self.AudioVisualizer=Button(text="Audio Visualizer",command=self.audio_command)#row 4
        self.weather=Button(text="Weather")
        self.screen=Button(text="Audio Visualizer")
        self.TBD1=Button(text="Audio Visualizer")
        self.TBD2=Button(text="Audio Visualizer")

        #pack row 1
        self.lightdisplay.grid(row=0)
        self.reset_button.grid(row=1, column=3)
        self.redval.grid(row=2,column=0)
        self.greenval.grid(row=3, column=0)
        self.blueval.grid(row=4,column=0)
        self.colorEnter.grid(row=5,column=0)
        self.TimerIn.grid(row=2,column=1)
        self.TimerCheck.grid(row=2,column=2)
        self.AudioVisualizer.grid(row=2,column=3)
        self.weather.grid(row=2,column=3)
        self.weather.grid(row=3,column=3)
        self.screen.grid(row=4,column=3)
        self.TBD1.grid(row=5,column=3)
        self.TBD2.grid(row=6,column=3)
        #end pack

    def __init__(self, master=None):
        self.audio_exit=False
        Frame.__init__(self, master)
        self.LEDNUM=0
        if(sys.platform=="linux"):
            try:
                self.ser=serial.Serial("/dev/tty.usbserial",9600)
            except:
                print("could not connect (linux)")
        else:
            try:
                self.ser=serial.Serial("COM4",9600)
            except:
                print("could not connect (Windows)")
        
        self.createWidgets()





if __name__=="__main__":
    root=Tk()
    try:
        ser=serial.Serial("COM4",9600)
    except:
        print("could not find arduino on COM4, check connection")
    mainWindow=GUI(master=root)
    mainWindow.mainloop()