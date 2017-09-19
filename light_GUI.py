from tkinter import *

class GUI(Frame):
    
    def createWidgets(self):
        self.lightdisplay=Canvas(self)

        self.redval=Spinbox(from_=0,to=255)#begin row 1
        self.greenval=Spinbox(from_=0,to=255)
        self.blueval=Spinbox(from_=0,to=255)
        self.colorEnter=Button(text="Display")#end row 1

        #pack row 1
        self.redval.grid(row=1,column=0)
        self.greenval.grid(row=2, column=0)
        self.blueval.grid(row=3,column=0)
        self.colorEnter.grid(row=4,column=0)
        #end pack

        self.TimerIn=Entry(self)#begin row 2
        self.TimerCheck=Button(self)#row 3

        #pack row2
        #end pack
        self.AudioVisualizer=Button(self)#row 4
        self.weather=Button(self)
        self.screen=Button(self)
        self.TBD1=Button(self)
        self.TBD2=Button(self)

        #pack row 3
        #end pack

    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.createWidgets()





if __name__=="__main__":
    root=Tk()
    mainWindow=GUI(master=root)
    mainWindow.mainloop()
    root.destroy()