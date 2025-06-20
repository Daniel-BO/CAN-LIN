import can

# Configurar bus CAN
bus = can.interface.Bus(channel='can0', bustype='socketcan')

# Crear y enviar mensaje
msg = can.Message(
    arbitration_id=0x123,
    data=[0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88],
    is_extended_id=False
)
bus.send(msg)

# Recibir mensajes
while True:
    message = bus.recv(1.0)  # Timeout de 1 segundo
    if message is not None:
        print(f"ID: {hex(message.arbitration_id)} Data: {message.data}")

bus.shutdown()
