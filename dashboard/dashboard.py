import streamlit as st
import serial
import serial.tools.list_ports
import time
import re


# ============================================================
# SMART MINE WORKER SAFETY AND MONITORING SYSTEM
# Streamlit Monitoring Dashboard
# ============================================================


# ============================================================
# PAGE CONFIGURATION
# ============================================================

st.set_page_config(
    page_title="Smart Mine Worker Safety",
    page_icon="⛑️",
    layout="wide"
)


# ============================================================
# THRESHOLDS
# ============================================================

GAS_THRESHOLD = 1000
TEMPERATURE_THRESHOLD = 45.0
HEART_RATE_HIGH = 120
HEART_RATE_LOW = 50
SPO2_THRESHOLD = 90


# ============================================================
# SESSION STATE
# ============================================================

if "serial_connection" not in st.session_state:
    st.session_state.serial_connection = None

if "latest_data" not in st.session_state:
    st.session_state.latest_data = None


# ============================================================
# TITLE
# ============================================================

st.title(
    "⛑️ Smart Mine Worker Safety "
    "and Monitoring System"
)

st.caption(
    "Real-Time Monitoring using ESP8266 + LoRa"
)


# ============================================================
# SIDEBAR
# ============================================================

st.sidebar.header(
    "Receiver Configuration"
)


# ------------------------------------------------------------
# SERIAL PORTS
# ------------------------------------------------------------

ports = list(
    serial.tools.list_ports.comports()
)


port_options = [
    port.device
    for port in ports
]


if not port_options:

    st.sidebar.warning(
        "No serial ports detected."
    )

    selected_port = None

else:

    selected_port = st.sidebar.selectbox(
        "Select Serial Port",
        port_options
    )


baud_rate = st.sidebar.selectbox(
    "Baud Rate",
    [9600, 115200],
    index=1
)


# ------------------------------------------------------------
# CONNECT BUTTON
# ------------------------------------------------------------

if st.sidebar.button(
    "Connect"
):

    if selected_port is None:

        st.sidebar.error(
            "No serial port available."
        )

    else:

        try:

            if (
                st.session_state.serial_connection
                and
                st.session_state.serial_connection.is_open
            ):

                st.session_state.serial_connection.close()


            st.session_state.serial_connection = serial.Serial(
                selected_port,
                baud_rate,
                timeout=1
            )

            time.sleep(2)

            st.sidebar.success(
                f"Connected to {selected_port}"
            )

        except Exception as e:

            st.sidebar.error(
                f"Connection failed: {e}"
            )


# ============================================================
# DISCONNECT
# ============================================================

if st.sidebar.button(
    "Disconnect"
):

    if (
        st.session_state.serial_connection
        and
        st.session_state.serial_connection.is_open
    ):

        st.session_state.serial_connection.close()

        st.session_state.serial_connection = None

        st.sidebar.info(
            "Disconnected."
        )


# ============================================================
# DATA PARSER
# ============================================================

def parse_packet(packet):

    data = {}

    parts = packet.strip().split(",")

    for part in parts:

        if ":" not in part:
            continue

        key, value = part.split(
            ":",
            1
        )

        data[key] = value

    return data


# ============================================================
# NUMBER HELPERS
# ============================================================

def safe_int(value, default=0):

    try:
        return int(float(value))

    except:
        return default


def safe_float(value, default=0.0):

    try:
        return float(value)

    except:
        return default


# ============================================================
# READ SERIAL DATA
# ============================================================

def read_serial_data():

    connection = (
        st.session_state.serial_connection
    )

    if (
        connection is None
        or
        not connection.is_open
    ):

        return None


    latest_packet = None


    try:

        # Read available lines
        while connection.in_waiting:

            line = (
                connection.readline()
                .decode(
                    "utf-8",
                    errors="ignore"
                )
                .strip()
            )


            # We only process worker packets
            if line.startswith(
                "ID:01"
            ):

                latest_packet = line


        if latest_packet:

            return parse_packet(
                latest_packet
            )


    except Exception as e:

        st.sidebar.error(
            f"Serial error: {e}"
        )


    return None


# ============================================================
# ALERT CALCULATION
# ============================================================

def calculate_alerts(data):

    gas = safe_int(
        data.get("GAS", 0)
    )

    environment_temp = safe_float(
        data.get("TEMP", 0)
    )

    body_temp = safe_float(
        data.get("BODYTEMP", 0)
    )

    heart_rate = safe_int(
        data.get("HR", 0)
    )

    spo2 = safe_int(
        data.get("SPO2", 0)
    )

    flame = data.get(
        "FLAME",
        "SAFE"
    )

    fall = data.get(
        "FALL",
        "NO"
    )

    panic = data.get(
        "PANIC",
        "NO"
    )


    gas_alert = (
        gas > GAS_THRESHOLD
    )


    temperature_alert = (
        environment_temp
        > TEMPERATURE_THRESHOLD
    )


    heart_rate_alert = (
        heart_rate != 0
        and
        (
            heart_rate > HEART_RATE_HIGH
            or
            heart_rate < HEART_RATE_LOW
        )
    )


    spo2_alert = (
        spo2 != 0
        and
        spo2 < SPO2_THRESHOLD
    )


    flame_alert = (
        flame == "FIRE"
    )


    fall_alert = (
        fall == "YES"
    )


    panic_alert = (
        panic == "YES"
    )


    overall_alert = (
        gas_alert
        or temperature_alert
        or heart_rate_alert
        or spo2_alert
        or flame_alert
        or fall_alert
        or panic_alert
    )


    return {
        "gas": gas_alert,
        "temperature": temperature_alert,
        "heart_rate": heart_rate_alert,
        "spo2": spo2_alert,
        "flame": flame_alert,
        "fall": fall_alert,
        "panic": panic_alert,
        "overall": overall_alert
    }


# ============================================================
# DISPLAY HELPERS
# ============================================================

def status_text(alert):

    return (
        "🚨 ALERT"
        if alert
        else
        "🟢 NORMAL"
    )


def status_box(label, value, alert=False):

    if alert:

        st.error(
            f"**{label}**  \n{value}"
        )

    else:

        st.success(
            f"**{label}**  \n{value}"
        )


# ============================================================
# READ DATA
# ============================================================

new_data = read_serial_data()


if new_data:

    st.session_state.latest_data = (
        new_data
    )


data = st.session_state.latest_data


# ============================================================
# DASHBOARD
# ============================================================

if data is None:

    st.info(
        "Waiting for data from the receiver..."
    )

    st.markdown(
        """
        ### How to use

        1. Connect the ESP8266 receiver to the computer.
        2. Select the corresponding serial port.
        3. Select the baud rate.
        4. Click **Connect**.
        5. Wait for LoRa packets from the helmet.
        """
    )

else:

    alerts = calculate_alerts(
        data
    )


    # ========================================================
    # SYSTEM STATUS
    # ========================================================

    st.subheader(
        "System Status"
    )


    if alerts["overall"]:

        st.error(
            "🚨 SYSTEM ALERT"
        )

    else:

        st.success(
            "🟢 SYSTEM NORMAL"
        )


    # ========================================================
    # WORKER INFORMATION
    # ========================================================

    st.subheader(
        "Worker Monitoring"
    )


    col1, col2, col3, col4 = st.columns(4)


    with col1:

        st.metric(
            "Worker ID",
            data.get(
                "ID",
                "Unknown"
            )
        )


    with col2:

        gas = safe_int(
            data.get(
                "GAS",
                0
            )
        )

        st.metric(
            "Gas Level",
            gas
        )


    with col3:

        temperature = safe_float(
            data.get(
                "TEMP",
                0
            )
        )

        st.metric(
            "Environment Temp",
            f"{temperature:.2f} °C"
        )


    with col4:

        body_temp = safe_float(
            data.get(
                "BODYTEMP",
                0
            )
        )

        st.metric(
            "Body Temp",
            f"{body_temp:.2f} °C"
        )


    # ========================================================
    # HEALTH MONITORING
    # ========================================================

    st.subheader(
        "Health Monitoring"
    )


    col1, col2, col3 = st.columns(3)


    with col1:

        hr = safe_int(
            data.get(
                "HR",
                0
            )
        )

        status_box(
            "Heart Rate",
            f"{hr} BPM",
            alerts["heart_rate"]
        )


    with col2:

        spo2 = safe_int(
            data.get(
                "SPO2",
                0
            )
        )

        status_box(
            "SpO₂",
            f"{spo2} %",
            alerts["spo2"]
        )


    with col3:

        fall = data.get(
            "FALL",
            "NO"
        )

        status_box(
            "Fall Detection",
            (
                "Fall Detected"
                if fall == "YES"
                else
                "No Fall"
            ),
            alerts["fall"]
        )


    # ========================================================
    # ENVIRONMENT MONITORING
    # ========================================================

    st.subheader(
        "Environmental Monitoring"
    )


    col1, col2, col3 = st.columns(3)


    with col1:

        gas = safe_int(
            data.get(
                "GAS",
                0
            )
        )

        status_box(
            "Gas",
            str(gas),
            alerts["gas"]
        )


    with col2:

        flame = data.get(
            "FLAME",
            "SAFE"
        )

        status_box(
            "Flame",
            (
                "🔥 FIRE DETECTED"
                if flame == "FIRE"
                else
                "No Fire"
            ),
            alerts["flame"]
        )


    with col3:

        temp = safe_float(
            data.get(
                "TEMP",
                0
            )
        )

        status_box(
            "Temperature",
            f"{temp:.2f} °C",
            alerts["temperature"]
        )


    # ========================================================
    # EMERGENCY STATUS
    # ========================================================

    st.subheader(
        "Emergency Status"
    )


    col1, col2, col3 = st.columns(3)


    with col1:

        panic = data.get(
            "PANIC",
            "NO"
        )

        status_box(
            "Panic Button",
            (
                "PANIC ACTIVE"
                if panic == "YES"
                else
                "Normal"
            ),
            alerts["panic"]
        )


    with col2:

        vibration = data.get(
            "VIB",
            "NO"
        )

        status_box(
            "Receiver Vibration",
            (
                "Vibration Detected"
                if vibration == "YES"
                else
                "Normal"
            ),
            vibration == "YES"
        )


    with col3:

        button = data.get(
            "BUTTON",
            "RELEASED"
        )

        status_box(
            "Receiver Button",
            button,
            button == "PRESSED"
        )


    # ========================================================
    # LORA SIGNAL
    # ========================================================

    st.subheader(
        "LoRa Communication"
    )


    rssi = safe_int(
        data.get(
            "RSSI",
            0
        )
    )


    if rssi >= -70:

        signal_status = "Excellent"

    elif rssi >= -85:

        signal_status = "Good"

    elif rssi >= -100:

        signal_status = "Weak"

    else:

        signal_status = "Very Weak"


    col1, col2 = st.columns(2)


    with col1:

        st.metric(
            "RSSI",
            f"{rssi} dBm"
        )


    with col2:

        st.metric(
            "Signal Status",
            signal_status
        )


    # ========================================================
    # RAW PACKET
    # ========================================================

    with st.expander(
        "View Received Data"
    ):

        st.code(
            ", ".join(
                f"{key}:{value}"
                for key, value
                in data.items()
            )
        )


# ============================================================
# AUTO REFRESH
# ============================================================

time.sleep(0.5)

st.rerun()
