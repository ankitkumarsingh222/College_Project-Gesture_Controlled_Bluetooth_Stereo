from machine import Pin
import time

# Define GPIO pins for the IR sensors
LEFT_SENSOR_PIN = 14
RIGHT_SENSOR_PIN = 15
left_op_pin=20
right_op_pin=21

# Initialize the IR sensors
left_sensor = Pin(LEFT_SENSOR_PIN, Pin.IN)
right_sensor = Pin(RIGHT_SENSOR_PIN, Pin.IN)
leftop = Pin(left_op_pin,Pin.OUT)
rightop = Pin(right_op_pin ,Pin.OUT)
# Function to detect gestures
def detect_gesture():
    while True:
        # Read sensor states (LOW when object is detected)
        left_detected = not left_sensor.value()
        right_detected = not right_sensor.value()

        # Detect left-to-right swipe (left sensor activates first, then right)
        if left_detected and not right_detected:
            time.sleep(0.2)  # Stabilize the reading
            if not right_sensor.value():  # Check if the right sensor is triggered
                print("Swipe Right Detected")
                rightop.on()
                time.sleep(0.5)  # Avoid duplicate detections

        # Detect right-to-left swipe (right sensor activates first, then left)
        elif right_detected and not left_detected:
            time.sleep(0.2)  # Stabilize the reading
            if not left_sensor.value():  # Check if the left sensor is triggered
                print("Swipe Left Detected")
                leftop.on()
                time.sleep(0.5)  # Avoid duplicate detections

        # Small delay to reduce CPU usage
        time.sleep(0.1)

# Main program loop
rightop.off()
leftop.off()
try:
    print("Starting gesture detection...")
    detect_gesture()
except KeyboardInterrupt:
    print("Program interrupted by user.")
