from audioop import minmax
from cProfile import label
import numpy as np
import matplotlib.pyplot as plt
import csv
import math

def distanceBetween( lat1,  long1,  lat2,  long2):

    delta = math.radians(long1-long2)
    sdlong = math.sin(delta)
    cdlong = math.cos(delta)
    lat1 = math.radians(lat1)
    lat2 = math.radians(lat2)
    slat1 = math.sin(lat1)
    clat1 = math.cos(lat1)
    slat2 = math.sin(lat2)
    clat2 = math.cos(lat2)
    delta = (clat1 * slat2) - (slat1 * clat2 * cdlong)
    delta = math.pow(delta,2)
    delta += math.pow(clat2 * sdlong,2)
    delta = math.sqrt(delta)
    denom = (slat1 * slat2) + (clat1 * clat2 * cdlong)
    delta = math.atan2(delta, denom)
    return delta * 6372795

hora = []
millis = []
pot = []
m1 = []
m2 = []
m3 = []
m4 = []
altura_consigna = []
altura = []
pitch_consigna = []
pitch = []
pitch_temp =[]
roll_consigna = []
roll = []
roll_temp =[]
yaw_consigna = []
yaw = []
bat = []
az = []
pulsoSW = []
pitch_P = []
pitch_I = []
pitch_D = []
roll_P = []
roll_I = []
roll_D = []
yaw_P = []
yaw_I = []
yaw_D = []
altura_P = []
altura_I = []
altura_D = []
num_satelites = []
senial_hdop = []
latitud = []
longitud = []
altura_gps = []
velocidad_gps = []
tiempo_GPS = []
distancia_GPS = []
latitud_real = []
longitud_real = []
altitud_real = []
velocidad_real = []
magx = []
magy = []
magz = []
direccion = []
P_latitud = []
I_latitud = []
D_latitud = []
P_longitud = []
I_longitud = []
D_longitud = []
latitud_cons = []
longitud_cons = []
latitud_inter = []
longitud_inter = []
latitud_metros = []
longitud_metros = []
latitud_cons_metros = []
longitud_cons_metros = []
auxx = []
auxy = []

with open('E:ARCHIVOS/ESTUDIOS/Universidad/2021-2022/TFG/Github/TFG/Python/hello/Serials/serial_20221129_110920.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
        hora.append((row[0]))
        millis.append(int(row[1]))
        pot.append(int(row[2]))
        m1.append(int(row[3]))
        m2.append(int(row[4]))
        m3.append(int(row[5]))
        m4.append(int(row[6]))
        altura_consigna.append(float(row[7]))
        altura.append( float(row[8]))
        pitch_consigna.append( float(row[9]))
        pitch.append( float(row[10]))
        pitch_temp.append( float(row[11]))
        roll_consigna.append( float(row[12]))
        roll.append( float(row[13]))
        roll_temp.append( float(row[14]))
        yaw_consigna.append( float(row[15]))
        yaw.append( float(row[16]))
        bat.append(float(row[17]))
        az.append( float(row[18]))
        pulsoSW.append( float(row[19]))
        pitch_P.append( float(row[20]))
        pitch_I.append( float(row[21]))
        pitch_D.append( float(row[22]))
        roll_P.append( float(row[23]))
        roll_I.append( float(row[24]))
        roll_D.append( float(row[25]))
        yaw_P.append( float(row[26]))
        yaw_I.append( float(row[27]))
        yaw_D.append( float(row[28]))
        altura_P.append( float(row[29]))
        altura_I.append( float(row[30]))
        altura_D.append( float(row[31]))
        num_satelites.append( float(row[32]))
        senial_hdop.append( float(row[33]))
        latitud.append( float(row[34])/1000000.0)
        longitud.append( float(row[35])/1000000.0)
        altura_gps.append( float(row[36]))
        velocidad_gps.append( float(row[37]))
         
        magx.append( float(row[38]))
        magy.append( float(row[39]))
        magz.append( float(row[40]))
        P_latitud.append(float(row[42]))
        I_latitud.append(float(row[43]))
        D_latitud.append(float(row[44]))
        P_longitud.append(float(row[45]))
        I_longitud.append(float(row[46]))
        D_longitud.append(float(row[47]))
        latitud_cons.append(float(row[48])/1000000.0)
        longitud_cons.append(float(row[49])/1000000.0)
        latitud_inter.append(float(row[50])/1000000.0)
        longitud_inter.append(float(row[51])/1000000.0)
        aux = math.atan2(float(row[39]),float(row[38]))*180/math.pi 
        auxx.append(float((row[38])) + 55)
        auxy.append((float((row[39])) - 632.5)*0.95)
        if(aux < 0):
            direccion.append(aux + 360)
        else:
            direccion.append(aux) 


minX = millis[0]
maxX = millis[len(millis)-1]

# minX = 370000
# maxX = 220860	
print(distanceBetween(0,0,0 ,0.00000001))

for n in range(len(millis)-1):


    if (latitud[n] > 0.0):
        tiempo_GPS.append(millis[n])
        distancia_GPS.append(distanceBetween(latitud_inter[n],longitud_inter[n],latitud_cons[n] ,longitud_cons[n]))
        latitud_real.append(latitud[n])
        longitud_real.append(longitud[n])
        velocidad_real.append(velocidad_gps[n])
        altitud_real.append(altura_gps[n])
        latitud_metros.append(distanceBetween(latitud_inter[n],0,latitud_cons[0] ,0))
        longitud_metros.append(distanceBetween(0,longitud_inter[n],0,longitud_cons[0]))
        latitud_cons_metros.append(distanceBetween(latitud_cons[n],0,latitud_cons[0] ,0))
        longitud_cons_metros.append(distanceBetween(0,longitud_cons[n],0,longitud_cons[0]))



with open('latitud', 'w+', newline='') as file:
    writer = csv.writer(file, delimiter='\n')
    writer.writerow(latitud)

with open('longitud', 'w+', newline='') as file:
    writer = csv.writer(file, delimiter='\n')
    writer.writerow(longitud)

'''with open('magX', 'w+', newline='') as file:
    writer = csv.writer(file, delimiter='\n')
    writer.writerow(magx)

with open('magY', 'w+', newline='') as file:
    writer = csv.writer(file, delimiter='\n')
    writer.writerow(magy)

with open('magZ', 'w+', newline='') as file:
    writer = csv.writer(file, delimiter='\n')
    writer.writerow(magz) '''
   

"""****************************************** ANGULOS ******************************************"""
fig1 = plt.figure(1,label='Angulos')

sub1 = plt.subplot(411)
plt.plot(millis, m1, label ='M1')
plt.plot(millis, m2, label = 'M2')
plt.plot(millis, m3, label = 'M3')
plt.plot(millis, m4, label = 'M4')
plt.plot(millis, pot, label = 'Throttle')
sub1.set_xlim(minX, maxX)
sub1.set_ylim(190, 415)
sub1.set_xlabel('Tiempo (ms)')
sub1.set_ylabel('PWM')
sub1.grid(True)
sub1.legend(title='PWM Motores', loc = "upper right")


sub2 = plt.subplot(412)
plt.plot(millis, roll_consigna, label = 'Consigna')
plt.plot(millis, roll, label = 'Salida')
sub2.set_xlim(minX, maxX)
sub2.set_ylim(-40, 40)
sub2.set_ylabel('Roll (grados)')
sub2.set_xlabel('Tiempo (ms)')
sub2.grid(True)
sub2.legend(title='Roll MPU', loc = "upper right")


sub3 = plt.subplot(413)
plt.plot(millis, pitch_consigna, label = 'Consigna')
plt.plot(millis, pitch, label = 'Salida')
sub3.set_xlim(minX, maxX)
sub3.set_ylim(-40, 40)
sub3.set_ylabel('Pitch (grados)')
sub3.set_xlabel('Tiempo (ms)')
sub3.grid(True)
sub3.legend(title='Pitch MPU', loc = "upper right")


sub4 = plt.subplot(414)
plt.plot(millis, yaw_consigna, label = 'Consigna')
plt.plot(millis, yaw, label = 'Salida')
sub4.set_xlim(minX, maxX)
sub4.set_xlabel('Tiempo (ms)')
sub4.set_ylabel('Yaw (grados)')
sub4.grid(True)
sub4.legend(title='Yaw MPU', loc = "upper right")


plt.subplots_adjust(left=0.055, bottom=0.079, right=0.955, top=0.98, wspace=0, hspace=0.448)

"""****************************************** ALTURA ******************************************"""
fig2 = plt.figure(2,label='Altura')

sub1 = plt.subplot(411)
plt.plot(millis, m1, label ='M1')
plt.plot(millis, m2, label = 'M2')
plt.plot(millis, m3, label = 'M3')
plt.plot(millis, m4, label = 'M4')
plt.plot(millis, pot, label = 'Throttle')
sub1.set_xlim(minX, maxX)
sub1.set_ylim(195, 410)
sub1.set_xlabel('Tiempo (ms)')
sub1.set_ylabel('PWM')
sub1.grid(True)
sub1.legend(title='Motores PWM', loc = "upper right")


sub2 = plt.subplot(412)
plt.plot(millis, altura_consigna, label = 'Consigna')
plt.plot(millis, altura, label ='Salida')
sub2.set_xlim(minX, maxX)
#sub2.set_ylim(0, 1000)
#sub2.set_ylim(635, 645)
sub2.set_xlabel('Tiempo (ms)')
sub2.set_ylabel('Altura (m)')
sub2.grid(True)
sub2.legend(title='Altura Barómetro', loc = "upper right")

sub3 = plt.subplot(413)
plt.plot(millis, altura_P, label ='P')
plt.plot(millis, altura_I, label ='I')
plt.plot(millis, altura_D, label ='D')
sub3.set_xlim(minX, maxX)
sub3.set_ylim(-30, 30)
sub3.set_xlabel('Tiempo (ms)')
sub3.set_ylabel('PWM')
sub3.grid(True)
sub3.legend(title='PID Altura', loc = "upper right")


sub4 = plt.subplot(414)
plt.plot(millis, bat, label = 'Bateria')
sub4.set_xlim(minX, maxX)
sub4.set_ylabel('Tensión (V)')
sub4.set_xlabel('Tiempo (ms)')
sub4.set_ylim(10, 12.6)
sub4.grid(True)
sub4.legend(title='Bateria', loc = "upper right") 

plt.subplots_adjust(left=0.055, bottom=0.079, right=0.955, top=0.98, wspace=0, hspace=0.448)

"""****************************************** PID MOTORES ******************************************"""
fig3 = plt.figure(3,label='PID Motores')

sub1 = plt.subplot(411)
plt.plot(millis, m1, label ='M1')
plt.plot(millis, m2, label = 'M2')
plt.plot(millis, m3, label = 'M3')
plt.plot(millis, m4, label = 'M4')
plt.plot(millis, pot, label = 'Throttle')
sub1.set_xlim(minX, maxX)
sub1.set_ylim(190, 415)
sub1.set_xlabel('Tiempo (ms)')
sub1.set_ylabel('PWM')
sub1.grid(True)
sub1.legend(title='Motores PWM', loc = "upper right")

sub2 = plt.subplot(412)
plt.plot(millis, roll_P, label = 'P')
plt.plot(millis, roll_I, label = 'I')
plt.plot(millis, roll_D, label = 'D')
sub2.set_xlim(minX, maxX)
sub2.set_ylim(-35,35)
sub2.set_ylabel('PWM Roll')
sub2.set_xlabel('Tiempo (ms)')
sub2.grid(True)
sub2.legend(title='PID Roll', loc = "upper right")

sub3 = plt.subplot(413)
plt.plot(millis, pitch_P, label = 'P')
plt.plot(millis, pitch_I, label = 'I')
plt.plot(millis, pitch_D, label = 'D')
sub3.set_xlim(minX, maxX)
sub3.set_ylim(-35,35)
sub3.set_ylabel('PWM Pitch')
sub3.set_xlabel('Tiempo (ms)')
sub3.grid(True)
sub3.legend(title='PID Pitch', loc = "upper right")


sub4 = plt.subplot(414)
plt.plot(millis, yaw_P, label = 'P')
plt.plot(millis, yaw_I, label = 'I')
plt.plot(millis, yaw_D, label = 'D')
sub4.set_xlim(minX, maxX)
sub4.set_ylim(-10,10)
sub4.set_ylabel('PWM Yaw')
sub4.set_xlabel('Tiempo (ms)')
sub4.grid(True)
sub4.legend(title='PID Yaw', loc = "upper right")

 
 
plt.subplots_adjust(left=0.055, bottom=0.079, right=0.955, top=0.98, wspace=0, hspace=0.448)

"""****************************************** GPS ******************************************"""
fig4 = plt.figure(4,label='GPS')
sub1 = plt.subplot(411)
#plt.plot(tiempo_GPS, latitud_real, label = 'latitud')
plt.plot(millis, latitud_cons, label = 'Consgina')
plt.plot(millis, latitud_inter, label = 'Salida')
sub1.set_xlim(minX, maxX)
sub1.set_ylabel('Latitud (º)')
sub1.set_xlabel('Tiempo (ms)')
#sub1.set_ylim(-10, 40)
sub1.grid(True)
sub1.legend(title='Latitud', loc = "upper right")

sub1 = plt.subplot(412)
#plt.plot(tiempo_GPS, longitud_real, label = 'longitud')
plt.plot(millis, longitud_cons, label = 'Consgina')
plt.plot(millis, longitud_inter, label = 'Salida')
sub1.set_xlim(minX, maxX)
sub1.set_ylabel('Longitud (º)')
sub1.set_xlabel('Tiempo (ms)')
#sub1.set_ylim(-10, 40)
sub1.grid(True)
sub1.legend(title='Longitud', loc = "upper right")

# sub2 = plt.subplot(413)
# plt.plot(tiempo_GPS, altitud_real, label = 'altura')
# sub2.set_xlim(minX, maxX)
# #sub2.set_ylim(250, 280)
# sub2.set_ylabel('Coordenadas')
# sub2.grid(True)
# sub2.legend(title='GPS')

# sub3 = plt.subplot(514)
# plt.plot(millis, num_satelites, label = 'satelites')
# plt.plot(millis, senial_hdop, label = 'hdop')
# sub3.set_xlim(minX, maxX)
# #sub3.set_ylim(0, 15)
# sub3.set_ylabel('Num Satelites y HDOP')
# sub3.grid(True)
# sub3.legend(title='GPS') 

sub4 = plt.subplot(413)
plt.plot(tiempo_GPS, velocidad_real, label = 'Velocidad')
sub4.set_xlim(minX, maxX)
sub4.set_ylim(-5, 30)
sub4.set_ylabel('Velocidad (Km/h)')
sub4.set_xlabel('Tiempo (ms)')
sub4.grid(True)
sub4.legend(title='Velocidad GPS', loc = "upper right")

sub5 = plt.subplot(414)
plt.plot(tiempo_GPS, distancia_GPS, label = 'Salida')
sub5.set_xlim(minX, maxX)
#sub5.set_ylim(0, 100)
sub5.set_ylabel('Distancia (m)')
sub5.set_xlabel('Tiempo (ms)')
sub5.grid(True)
sub5.legend(title='Distancia al origen', loc = "upper right")

plt.subplots_adjust(left=0.055, bottom=0.079, right=0.955, top=0.98, wspace=0, hspace=0.448)
 
 
"""****************************************** MAGNETOMETRO ******************************************"""

fig5 = plt.figure(6,label='Magnetometro')
sub1 = plt.subplot(311)
plt.plot(magx, magy, label = 'Calibracion')
sub1.set_ylabel('Raw')
#sub1.set_ylim(-10, 40)
sub1.grid(True)
sub1.legend(title='Calibracion')

sub2 = plt.subplot(312)
plt.plot(millis, direccion, label = 'Grados')
sub2.set_xlim(minX, maxX)
#sub2.set_ylim(250, 280)
sub2.set_ylabel('Grados')
sub2.grid(True)
sub2.legend(title='Magnetometro')

sub3 = plt.subplot(313)
plt.plot(millis, pulsoSW, label = 'SW')
sub3.set_xlim(minX, maxX)
sub3.set_ylim(990, 2010)
sub3.set_ylabel('SW')
sub3.grid(True)
sub3.legend(title='SW')

fig5 = plt.figure(10,label='Magnetometro')
sub1 = plt.subplot(121)
plt.plot(magx, magy, label = 'Datos')
sub1.set_ylabel('Yh (mgauss)')
sub1.set_xlabel('Xh (mgauss)')
#sub1.set_ylim(-10, 40)
sub1.grid(True)
sub1.legend()

sub2 = plt.subplot(122)
plt.plot(auxx, auxy, label = 'Compensación', color = "orange")
#sub2.set_ylabel('Yh (mgauss)')
sub2.set_xlabel('Xh (mgauss)')
#sub1.set_ylim(-10, 40)
sub2.grid(True)
sub2.legend()


"""****************************************** GPS PID ******************************************"""
fig7 = plt.figure(7,label='GPS PID')

sub1 = plt.subplot(211)
plt.plot(millis, P_latitud, label = 'P')
plt.plot(millis, I_latitud, label = 'I')
plt.plot(millis, D_latitud, label = 'D')
sub1.set_ylabel('PWM')
sub1.set_xlabel('Tiempo (ms)')
#sub1.set_ylim(-10, 40)
sub1.grid(True)
sub1.legend(title='PID Latitud')

sub2 = plt.subplot(212)
plt.plot(millis, P_longitud, label = 'P')
plt.plot(millis, I_longitud, label = 'I')
plt.plot(millis, D_longitud, label = 'D')
sub2.set_xlim(minX, maxX)
#sub2.set_ylim(250, 280)
sub2.set_ylabel('PWM')
sub2.set_xlabel('Tiempo (ms)')
sub2.grid(True)
sub2.legend(title='PID Longitud', loc = "upper right")

plt.subplots_adjust(left=0.055, bottom=0.079, right=0.955, top=0.98, wspace=0, hspace=0.448)

"""****************************************** MAPA 2D ******************************************"""
fig8 = plt.figure(8,label='Mapa 2D')
sub8 = plt.subplot(111)
plt.plot(longitud, latitud, label = 'Posicion')
plt.plot(longitud_cons, latitud_cons, label = 'Consigna')
#plt.plot(longitud_metros, latitud_metros, label = 'Posicion')
#plt.plot(longitud_cons_metros, latitud_cons_metros, label = 'Consigna')
sub8.set_ylabel('Latitud')
sub8.set_xlabel('Longitud')
sub8.grid(True)
sub8.legend(title='Posicion 2D', loc = "upper right") 


plt.subplots_adjust(left=0.055, bottom=0.079, right=0.955, top=0.98, wspace=0, hspace=0.448)
plt.tight_layout()
plt.show()








