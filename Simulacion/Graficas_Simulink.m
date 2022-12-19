clc,close all


figure("Name",'Angulos')
hold on
plot(out.Roll)
plot(out.Pitch)
plot(out.Yaw)
legend(["Roll" "Pitch" "Yaw"])
title('Angulos')
xlabel('Tiempo (s)')
ylabel('Rotación (grados)')
grid on


figure("Name",'GPS')
hold on
plot(out.Lat)
plot(out.Long)
legend(["X" "Y"])
title('GPS')
xlabel('Tiempo (s)')
ylabel('Metros')
grid on

figure("Name",'Altura')
hold on
plot(out.Altura)
legend(["Altura"])
title('Altura')
xlabel('Tiempo (s)')
ylabel('Altura (m)')
grid on

figure("Name",'2D')
hold on
plot3(out.Lat.Data,out.Long.Data,out.Altura.Data)
title('Mapa 2D')
xlabel('X (m)')
ylabel('Y(m)')
zlabel('Z(m)')
grid on

figure("Name",'Angulos-Altura')
tiledlayout(2,1)
nexttile
hold on
plot(out.Lat)
plot(out.Long)
legend(["X" "Y"])
title('GPS')
xlabel('Tiempo (s)')
ylabel('Distancia (m)')
grid on
hold off
title('Angulos')

nexttile
plot(out.Altura.Time,out.Altura.Data)
title('Altura')
ylabel('Altura (m)')
xlabel('Tiempo (s)')
title('Altura')