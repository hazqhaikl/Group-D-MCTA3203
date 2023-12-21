import cv2
import numpy as np

def color_detection(frame):
    # Convert the frame from BGR to HSV color space
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Define the lower and upper bounds for red color in HSV
    lower_red = np.array([0, 100, 100])
    upper_red = np.array([10, 255, 255])
    red_mask1 = cv2.inRange(hsv_frame, lower_red, upper_red)

    lower_red = np.array([160, 100, 100])
    upper_red = np.array([180, 255, 255])
    red_mask2 = cv2.inRange(hsv_frame, lower_red, upper_red)

    # Combine the two red masks
    red_mask = cv2.bitwise_or(red_mask1, red_mask2)

    # Define the lower and upper bounds for yellow color in HSV
    lower_yellow = np.array([20, 100, 100])
    upper_yellow = np.array([30, 255, 255])
    yellow_mask = cv2.inRange(hsv_frame, lower_yellow, upper_yellow)

    # Define the lower and upper bounds for blue color in HSV
    lower_blue = np.array([90, 100, 100])
    upper_blue = np.array([130, 255, 255])
    blue_mask = cv2.inRange(hsv_frame, lower_blue, upper_blue)

    # Combine all color masks
    color_mask = cv2.bitwise_or(red_mask, yellow_mask, blue_mask)

    # Apply the color mask to the original frame
    result_frame = cv2.bitwise_and(frame, frame, mask=color_mask)

    # Count the number of non-zero pixels for each color
    red_count = np.count_nonzero(red_mask)
    yellow_count = np.count_nonzero(yellow_mask)
    blue_count = np.count_nonzero(blue_mask)

    # Determine the dominant color
    max_count = max(red_count, yellow_count, blue_count)
    if max_count == red_count:
        dominant_color = "Red"
    elif max_count == yellow_count:
        dominant_color = "Yellow"
    else:
        dominant_color = "Blue"

    # Display the frequency and dominant color
    cv2.putText(result_frame, f"Dominant Color: {dominant_color}", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)

    return result_frame

def capture_and_detect_color(camera_index=0):
    # Open a connection to the camera
    cap = cv2.VideoCapture(camera_index)

    # Check if the camera opened successfully
    if not cap.isOpened():
        print("Error: Could not open camera.")
        return

    try:
        while True:
            # Capture frame-by-frame
            ret, frame = cap.read()

            # Check if the frame was read successfully
            if not ret:
                print("Error: Failed to capture frame.")
                break

            # Detect color in the frame
            result_frame = color_detection(frame)

            # Display the original frame
            cv2.imshow("Original Video Feed", frame)

            # Display the frame with color detection
            cv2.imshow("Color Detection", result_frame)

            # Break the loop if 'q' key is pressed
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    finally:
        # Release the camera and close all OpenCV windows
        cap.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    # You can specify the camera index (0 is usually the default USB camera)
    camera_index = 0

    # Call the function to capture video and detect color
    capture_and_detect_color(camera_index)
