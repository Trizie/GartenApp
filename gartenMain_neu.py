import tkinter as tk
from tkinter import ttk
import Config
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
import matplotlib.animation as animation
from matplotlib import style
from btlewrap.bluepy import BluepyBackend
from miflora.miflora_poller import MiFloraPoller, MI_CONDUCTIVITY, MI_MOISTURE, MI_LIGHT, MI_TEMPERATURE, MI_BATTERY


LARGE_FONT = ("Verdana", 12)
style.use("ggplot")

fig_Sensor = Figure(figsize=(6,4), dpi=300)
a = fig_Sensor.add_subplot(111)


fig_Blue = Figure(figsize=(6,4), dpi=200)
b= fig_Blue.add_subplot(111)

php_param = "C4:7C:8D:6E:03:87"
poller = MiFloraPoller(php_param, BluepyBackend)

def animate(i):

    xa=Config.printDate()
    ya=Config.printBodenfeuchte()

    a.clear()
    a.plot(xa, ya)



def animateBlue(i):
    xb = Config.printDateB()
    yb = Config.printFeuchteB()

    b.clear()
    b.plot(xb, yb)
    print("aufgerufen")


class GartenApp(tk.Tk):

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        container = tk.Frame(self)

        container.pack(side="top", fill="both", expand=True)

        container.grid_rowconfigure(0, weight=1, minsize=500)
        container.grid_columnconfigure(0, weight=1, minsize=500)

        self.frames = {}

        for F in (Startseite, DatenScreen, SteuerScreen, BluetoothScreen):
            frame = F(container, self)

            self.frames[F] = frame

            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame(Startseite)

        menu = tk.Menu(self)
        self.config(menu=menu)
        mainmenu = tk.Menu(menu)
        menu.add_cascade(label="Main", menu=mainmenu)
        mainmenu.add_command(label="Hauptmenu", command=lambda: self.show_frame(Startseite))
        mainmenu.add_separator()
        mainmenu.add_command(label="Sensor", command=lambda: self.show_frame(DatenScreen))
        mainmenu.add_command(label="Bluetooth Sensor", command=lambda: self.show_frame(BluetoothScreen))
        mainmenu.add_separator()
        mainmenu.add_command(label="Steuern", command=lambda: self.show_frame(SteuerScreen))
        mainmenu.add_separator()
        mainmenu.add_command(label="Kamera")

    def show_frame(self, cont):
        frame = self.frames[cont]
        frame.tkraise()


class Startseite(tk.Frame):

    def __init__(self, parent, controller):
        ttk.Frame.__init__(self, parent)
        label = tk.Label(self, text="GartenApp", font=LARGE_FONT)
        label.pack(pady=10, padx=10)

        button = ttk.Button(self, text="Werte auslesen",
                            command=lambda: controller.show_frame(DatenScreen))
        button.pack()

        button2 = ttk.Button(self, text="Steuerung",
                             command=lambda: controller.show_frame(SteuerScreen))
        button2.pack()


class DatenScreen(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        label = tk.Label(self, text="Diagrammdarstellung", font=LARGE_FONT)
        label.pack(pady=10, padx=10)

        #fig = Figure(figsize=(10, 10),
                     #dpi=100)
        canvas = FigureCanvasTkAgg(fig_Sensor,
                                   master=self)
        #ax = fig.add_subplot(211)
        #ax1 = fig.add_subplot(212)
        #ax.plot(Config.printDate(), Config.printBodenfeuchte())
        #ax1.plot(Config.printDateTemp(), Config.printTemperatur())
        #ax.set_ylabel("Bodenfeuchtigkeit")
        #ax1.set_ylabel("Temperatur")
        canvas.draw()
        canvas.get_tk_widget().pack()



        #def clear():
         #   fig_Sensor.delaxes()

        #button1 = tk.Button(self, text="Bodenfeuchte",
                            #command=lambda:clear())
        #button1.pack()


class SteuerScreen(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = tk.Label(self, text="Steuerung", font=LARGE_FONT)
        label.pack(pady=10, padx=10)

        button1 = tk.Button(self, text="Pumpe aus",
                            command=Config.pumpeAus)
        button1.pack()

        button2 = tk.Button(self, text="Pumpe an",
                            command=Config.pumpeAn)
        button2.pack()

class BluetoothScreen(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        label = tk.Label(self, text="Diagrammdarstellung", font=LARGE_FONT)
        label.pack(pady=10, padx=10)

        canvas = FigureCanvasTkAgg(fig_Blue,
                                   master=self)
        #ax = fig.add_subplot(211)
        #ax1 = fig.add_subplot(212)
        #ax.plot(Config.printFeuchteB())
        #ax1.plot(Config.printDateTemp(), Config.printTemperatur())
        #ax.set_ylabel("Bodenfeuchtigkeit")
        #ax1.set_ylabel("Temperatur")
        canvas.draw()
        canvas.get_tk_widget().pack()

        label = tk.Label(self, text=format(poller.parameter_value(MI_TEMPERATURE)), font=LARGE_FONT)
        label.pack(pady=10, padx=10)

        button1 = tk.Button(self, text="Werte speichern",
                            command=Config.blueSens)
        button1.pack()


app = GartenApp()
ani = animation.FuncAnimation(fig_Sensor, animate, interval=1000)
ani_Blue = animation.FuncAnimation(fig_Blue,animateBlue, interval=1000)
app.mainloop()
