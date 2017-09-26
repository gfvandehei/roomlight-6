'''
Author: Gabriel Vande Hei
Purpose: create an easy interface with the arduino Uno, to abstract the
         control of LED's using WS1812B LED controller. Look at arducode.Ino
         for arduino control script
'''

from tkinter import *
import serial

class GUI(Frame):
    '''def colorCommand(self):
        self.TimerIn.insert(INSERT, "COLOR")
'''
    def createWidgets(self):
        self.lightdisplay=Canvas(height=30, width=300)
        self.redval=Spinbox(from_=0,to=255)#begin row 1
        self.greenval=Spinbox(from_=0,to=255)
        self.blueval=Spinbox(from_=0,to=255)
        self.LEDSelector=Spinbox(from_=0,to=self.LEDNUM)
        self.colorEnter=Button(text="Display")#end row 1
        self.TimerIn=Entry()#begin row 2
        self.TimerCheck=Button(text="Start")#row 3
        self.AudioVisualizer=Button(text="Audio Visualizer")#row 4
        self.weather=Button(text="         ?          ")
        self.screen=Button(text="    Weather    ")
        self.TBD1=Button(text="         ?        ")
        self.TBD2=Button(text="         ?        ")

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
        Frame.__init__(self, master)
        self.LEDNUM=0
        self.createWidgets()





if __name__=="__main__":
    root=Tk()
    try:
        ser=serial.Serial("COM4",9600)
    except:
        print("could not find arduino on COM4, check connection")
    mainWindow=GUI(master=root)
    mainWindow.mainloop()
    root.destroy()