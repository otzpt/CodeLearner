/*
 * Arduino.h - a stand-in for the Arduino core, so a sketch builds and runs
 * on a PC.
 *
 * This is NOT the real core. It reimplements the part of the Arduino API
 * the course uses, with the behaviour of an Arduino Uno (ATmega328P): the
 * pin-to-port mapping, the PORTx/DDRx/PINx registers, analogWrite() falling
 * back to digital on non-PWM pins, and so on. Where the PC cannot behave like
 * the chip (an int is 4 bytes here, 2 on the Uno; a double is 8 here, 4
 * there), the course says so out loud instead of hiding it.
 *
 * Everything whose name starts with sim_ is NOT part of Arduino. It is how a
 * lesson plays the part of the outside world: pressing a button, putting a
 * voltage on an analog pin, letting time pass. On a real board those things
 * happen by themselves.
 *
 * Time is virtual: delay() moves a clock forward instead of sleeping, so
 * "wait one second" costs nothing. The course says so in its time module.
 */

#ifndef ARDUINO_H_HOST
#define ARDUINO_H_HOST

#include <ctype.h>
#include <math.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <type_traits>

#define CODELEARNER_HOST 1

typedef uint8_t byte;
typedef bool boolean;

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define LED_BUILTIN 13

#define CHANGE 1
#define FALLING 2
#define RISING 3
#define NOT_AN_INTERRUPT (-1)

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

constexpr uint8_t A0 = 14, A1 = 15, A2 = 16, A3 = 17, A4 = 18, A5 = 19;

/* On an Uno only pins 2 and 3 can raise an external interrupt. */
#define digitalPinToInterrupt(p) ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#define PROGMEM
#define PSTR(x) (x)
#define F(x) (x)
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#define pgm_read_word(addr) (*(const uint16_t *)(addr))

#define _BV(bit) (1 << (bit))
#define bit(b) (1UL << (b))
#define bitRead(value, b) (((value) >> (b)) & 0x01)
#define bitSet(value, b) ((value) |= (1UL << (b)))
#define bitClear(value, b) ((value) &= ~(1UL << (b)))
#define bitWrite(value, b, bitvalue) ((bitvalue) ? bitSet(value, b) : bitClear(value, b))
#define lowByte(w) ((uint8_t)((w) & 0xff))
#define highByte(w) ((uint8_t)((w) >> 8))
#define sq(x) ((x) * (x))
#define degrees(rad) ((rad) * 57.295779513082320876798154814105)
#define radians(deg) ((deg) * 0.017453292519943295769236907684886)

template <class A, class B> inline auto min(A a, B b) -> decltype(a < b ? a : b)
{
    return a < b ? a : b;
}
template <class A, class B> inline auto max(A a, B b) -> decltype(a < b ? b : a)
{
    return a < b ? b : a;
}
template <class T, class L, class H> inline T constrain(T x, L low, H high)
{
    return x < low ? (T) low : (x > high ? (T) high : x);
}

/* The real map() works on 32-bit longs. int32_t keeps that true here, where
 * a long would be 64 bits. */
inline int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline long random(long max_value) { return max_value > 0 ? rand() % max_value : 0; }
inline long random(long min_value, long max_value)
{
    return min_value >= max_value ? min_value : min_value + random(max_value - min_value);
}
inline void randomSeed(unsigned long seed) { srand((unsigned) seed); }

/* ---- registers --------------------------------------------------------- */

/* The ATmega328P ports an Uno exposes. D: pins 0-7, B: pins 8-13 (bits 0-5),
 * C: pins A0-A5 = 14-19 (bits 0-5). */
inline volatile uint8_t DDRB = 0, PORTB = 0;
inline volatile uint8_t DDRC = 0, PORTC = 0;
inline volatile uint8_t DDRD = 0, PORTD = 0;
/* The core turns interrupts on (the I flag, bit 7) before setup() runs. */
inline volatile uint8_t SREG = 0x80;

/* What the outside world drives onto each pin: -1 = nothing connected. */
inline int8_t sim_ext[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
inline int sim_adc[6] = {0, 0, 0, 0, 0, 0};
inline uint8_t sim_pwm[20] = {0};
inline uint32_t sim_pulse_us[20] = {0};

struct SimPin {
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    uint8_t bit;
};

inline bool sim_lookup(uint8_t pin, SimPin &out)
{
    if (pin <= 7) {
        out = {&DDRD, &PORTD, pin};
    } else if (pin <= 13) {
        out = {&DDRB, &PORTB, (uint8_t)(pin - 8)};
    } else if (pin <= 19) {
        out = {&DDRC, &PORTC, (uint8_t)(pin - 14)};
    } else {
        return false;
    }
    return true;
}

/* The level a pin reads as. An output reads back what it drives. An input
 * reads what the outside drives; with nothing connected it reads HIGH if the
 * pull-up is on and LOW otherwise (hardware would float; LOW keeps this
 * repeatable). */
inline uint8_t sim_level(uint8_t pin)
{
    SimPin p;
    if (!sim_lookup(pin, p)) {
        return LOW;
    }
    uint8_t port_bit = (*p.port >> p.bit) & 1;
    if ((*p.ddr >> p.bit) & 1) {
        return port_bit;
    }
    return sim_ext[pin] >= 0 ? (uint8_t) sim_ext[pin] : port_bit;
}

inline uint8_t sim_pinreg(uint8_t first_pin, uint8_t bits)
{
    uint8_t value = 0;
    for (uint8_t i = 0; i < bits; i++) {
        value |= (uint8_t)(sim_level((uint8_t)(first_pin + i)) << i);
    }
    return value;
}

/* PINx reads the pins. It is a macro because the real one is a register the
 * hardware updates, not a variable the program writes. */
#define PINB sim_pinreg(8, 6)
#define PINC sim_pinreg(14, 6)
#define PIND sim_pinreg(0, 8)

inline void pinMode(uint8_t pin, uint8_t mode)
{
    SimPin p;
    if (!sim_lookup(pin, p)) {
        return;
    }
    if (mode == OUTPUT) {
        *p.ddr |= (uint8_t)(1 << p.bit);
    } else {
        *p.ddr &= (uint8_t) ~(1 << p.bit);
        if (mode == INPUT_PULLUP) {
            *p.port |= (uint8_t)(1 << p.bit);
        } else {
            *p.port &= (uint8_t) ~(1 << p.bit);
        }
    }
}

/* Writing the PORT bit of an INPUT pin turns its pull-up on or off. That is
 * how the real chip behaves, and the real core does not stop it. */
inline void digitalWrite(uint8_t pin, uint8_t value)
{
    SimPin p;
    if (!sim_lookup(pin, p)) {
        return;
    }
    if (value) {
        *p.port |= (uint8_t)(1 << p.bit);
    } else {
        *p.port &= (uint8_t) ~(1 << p.bit);
    }
}

inline int digitalRead(uint8_t pin) { return sim_level(pin); }

inline int analogRead(uint8_t pin)
{
    uint8_t channel = pin >= 14 ? (uint8_t)(pin - 14) : pin;
    return channel < 6 ? sim_adc[channel] : 0;
}

inline bool sim_is_pwm_pin(uint8_t pin)
{
    return pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11;
}

/* On a PWM pin: a duty cycle 0-255. On any other pin: HIGH at 128 or more,
 * LOW below, the same fallback the real core has. */
inline void analogWrite(uint8_t pin, int value)
{
    pinMode(pin, OUTPUT);
    if (sim_is_pwm_pin(pin)) {
        sim_pwm[pin] = (uint8_t) constrain(value, 0, 255);
        digitalWrite(pin, value >= 128 ? HIGH : LOW);
    } else {
        digitalWrite(pin, value >= 128 ? HIGH : LOW);
    }
}

inline uint32_t sim_pulse_in(uint8_t pin) { return sim_pulse_us[pin]; }
inline uint32_t pulseIn(uint8_t pin, uint8_t, unsigned long = 1000000UL)
{
    return sim_pulse_in(pin);
}

/* ---- time -------------------------------------------------------------- */

inline uint32_t sim_ms = 0;
inline uint32_t sim_us = 0;

inline uint32_t millis() { return sim_ms; }
inline uint32_t micros() { return sim_ms * 1000u + sim_us; }
inline void delay(unsigned long ms) { sim_ms += (uint32_t) ms; }
inline void delayMicroseconds(unsigned int us)
{
    sim_us += us;
    while (sim_us >= 1000) {
        sim_us -= 1000;
        sim_ms++;
    }
}
inline void sim_set_time(uint32_t ms)
{
    sim_ms = ms;
    sim_us = 0;
}

/* ---- interrupts -------------------------------------------------------- */

inline void (*sim_isr[2])() = {nullptr, nullptr};
inline uint8_t sim_isr_mode[2] = {0, 0};
inline bool sim_isr_pending[2] = {false, false};

inline void sim_run_isr(int n)
{
    uint8_t saved = SREG;
    SREG = (uint8_t)(SREG & 0x7F); /* the I flag is cleared while an ISR runs */
    sim_isr[n]();
    SREG = saved;
}

inline void attachInterrupt(uint8_t n, void (*handler)(), int mode)
{
    if (n < 2) {
        sim_isr[n] = handler;
        sim_isr_mode[n] = (uint8_t) mode;
    }
}
inline void detachInterrupt(uint8_t n)
{
    if (n < 2) {
        sim_isr[n] = nullptr;
    }
}

inline void interrupts()
{
    SREG = (uint8_t)(SREG | 0x80);
    for (int n = 0; n < 2; n++) {
        if (sim_isr_pending[n] && sim_isr[n]) {
            sim_isr_pending[n] = false;
            sim_run_isr(n);
        }
    }
}
inline void noInterrupts() { SREG = (uint8_t)(SREG & 0x7F); }
inline void sei() { interrupts(); }
inline void cli() { noInterrupts(); }

/* Put a level on a pin from outside, as a button or a sensor would. If the
 * pin is 2 or 3 and an interrupt is attached for that edge, the interrupt
 * fires now, or stays pending until interrupts are enabled again. */
inline void sim_set_external(uint8_t pin, uint8_t level)
{
    uint8_t before = sim_level(pin);
    sim_ext[pin] = (int8_t) level;
    uint8_t after = sim_level(pin);

    if ((pin != 2 && pin != 3) || before == after) {
        return;
    }
    int n = pin - 2;
    uint8_t mode = sim_isr_mode[n];
    bool fire = mode == CHANGE || (mode == RISING && after == HIGH) ||
                (mode == FALLING && after == LOW);
    if (!fire || sim_isr[n] == nullptr) {
        return;
    }
    if (SREG & 0x80) {
        sim_run_isr(n);
    } else {
        sim_isr_pending[n] = true;
    }
}

inline void sim_set_analog(uint8_t channel, int value)
{
    if (channel < 6) {
        sim_adc[channel] = (int) constrain(value, 0, 1023);
    }
}
inline void sim_set_pulse(uint8_t pin, uint32_t microseconds) { sim_pulse_us[pin] = microseconds; }

/* ---- String ------------------------------------------------------------ */

class String {
  public:
    std::string s;

    String() {}
    String(const char *c) : s(c ? c : "") {}
    String(char c) : s(1, c) {}
    template <class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
    String(T v, int base = 10)
    {
        char buffer[72];
        if (base == 10) {
            snprintf(buffer, sizeof buffer, std::is_signed<T>::value ? "%lld" : "%llu",
                     std::is_signed<T>::value ? (long long) v : (long long) (unsigned long long) v);
        } else {
            unsigned long long u = (unsigned long long) (typename std::make_unsigned<T>::type) v;
            int n = 0;
            char tmp[72];
            do {
                tmp[n++] = "0123456789ABCDEF"[u % (unsigned) base];
                u /= (unsigned) base;
            } while (u);
            for (int i = 0; i < n; i++) {
                buffer[i] = tmp[n - 1 - i];
            }
            buffer[n] = '\0';
        }
        s = buffer;
    }
    String(double d, int decimals = 2)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%.*f", decimals, d);
        s = buffer;
    }

    unsigned int length() const { return (unsigned int) s.size(); }
    const char *c_str() const { return s.c_str(); }
    char charAt(unsigned int i) const { return i < s.size() ? s[i] : 0; }
    char operator[](unsigned int i) const { return charAt(i); }
    bool reserve(unsigned int) { return true; }
    long toInt() const { return strtol(s.c_str(), nullptr, 10); }
    float toFloat() const { return (float) atof(s.c_str()); }
    bool equals(const String &o) const { return s == o.s; }
    bool startsWith(const String &o) const { return s.compare(0, o.s.size(), o.s) == 0; }
    bool endsWith(const String &o) const
    {
        return s.size() >= o.s.size() && s.compare(s.size() - o.s.size(), o.s.size(), o.s) == 0;
    }
    int indexOf(char c, unsigned int from = 0) const
    {
        size_t at = s.find(c, from);
        return at == std::string::npos ? -1 : (int) at;
    }
    int indexOf(const String &o, unsigned int from = 0) const
    {
        size_t at = s.find(o.s, from);
        return at == std::string::npos ? -1 : (int) at;
    }
    String substring(unsigned int from) const
    {
        return from >= s.size() ? String() : String(s.substr(from).c_str());
    }
    String substring(unsigned int from, unsigned int to) const
    {
        if (from >= s.size() || to <= from) {
            return String();
        }
        return String(s.substr(from, to - from).c_str());
    }
    void trim()
    {
        size_t a = 0, b = s.size();
        while (a < b && isspace((unsigned char) s[a])) {
            a++;
        }
        while (b > a && isspace((unsigned char) s[b - 1])) {
            b--;
        }
        s = s.substr(a, b - a);
    }
    void toUpperCase()
    {
        for (auto &c : s) {
            c = (char) toupper((unsigned char) c);
        }
    }
    void toLowerCase()
    {
        for (auto &c : s) {
            c = (char) tolower((unsigned char) c);
        }
    }
    String &operator+=(const String &o)
    {
        s += o.s;
        return *this;
    }
    String &operator+=(const char *c)
    {
        s += c ? c : "";
        return *this;
    }
    String &operator+=(char c)
    {
        s += c;
        return *this;
    }
    /* A number appends its digits, not the character with that code. */
    template <class T, class = typename std::enable_if<std::is_arithmetic<T>::value &&
                                                       !std::is_same<T, char>::value>::type>
    String &operator+=(T v)
    {
        s += String(v).s;
        return *this;
    }
    bool operator==(const String &o) const { return s == o.s; }
    bool operator!=(const String &o) const { return s != o.s; }
    bool operator==(const char *c) const { return s == (c ? c : ""); }
    bool operator!=(const char *c) const { return s != (c ? c : ""); }
};

inline String operator+(const String &a, const String &b)
{
    String r = a;
    r += b;
    return r;
}
inline String operator+(const String &a, const char *b)
{
    String r = a;
    r += b;
    return r;
}
inline String operator+(const char *a, const String &b)
{
    String r(a);
    r += b;
    return r;
}
template <class T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
inline String operator+(const String &a, T b)
{
    return a + String(b);
}

/* ---- Serial ------------------------------------------------------------ */

inline void sim_quit()
{
    fflush(stdout);
    exit(0);
}

class SimSerial {
  public:
    void begin(unsigned long) {}
    void end() {}
    void setTimeout(unsigned long ms) { timeout_ms = (int) ms; }
    void flush() { fflush(stdout); }
    explicit operator bool() const { return true; }

    size_t print(const char *c) { return emit(c ? c : ""); }
    size_t print(char c) { return emit(std::string(1, c)); }
    size_t print(unsigned char v, int base = DEC) { return emit(number(v, base)); }
    size_t print(int v, int base = DEC) { return emit(number((long long) v, base, sizeof(int))); }
    size_t print(unsigned int v, int base = DEC) { return emit(number(v, base)); }
    size_t print(long v, int base = DEC) { return emit(number((long long) v, base, sizeof(long))); }
    size_t print(unsigned long v, int base = DEC) { return emit(number(v, base)); }
    size_t print(double v, int digits = 2) { return emit(real(v, digits)); }
    size_t print(const String &v) { return emit(v.s); }

    template <class T> size_t println(T v) { return print(v) + emit("\n"); }
    template <class T, class U> size_t println(T v, U extra) { return print(v, extra) + emit("\n"); }
    size_t println() { return emit("\n"); }

    size_t write(uint8_t c) { return emit(std::string(1, (char) c)); }
    size_t write(const char *c) { return print(c); }

    int available()
    {
        if (peek_wait(0) < 0) {
            usleep(1000); /* do not spin the PC while a sketch polls */
            return 0;
        }
        return (int) (buffer.size() - at);
    }
    int peek() { return peek_wait(0); }
    int read()
    {
        int c = peek_wait(0);
        if (c >= 0) {
            at++;
        }
        return c;
    }

    /* The real one gives up after the timeout (1 second unless setTimeout
     * says otherwise) and returns what it has. This one waits as long as it
     * takes, so a person at a keyboard is never cut off mid-answer, and ends
     * the program when the input ends (Ctrl-D). */
    String readStringUntil(char terminator)
    {
        String out;
        for (;;) {
            int c = peek_wait(-1);
            at++;
            if (c == terminator) {
                return out;
            }
            out += (char) c;
        }
    }

    /* Skips anything that is not a digit, then reads digits. Honours the
     * timeout like the real one: no number within it returns 0. */
    long parseInt()
    {
        int c;
        while ((c = peek_wait(timeout_ms)) >= 0 && c != '-' && !isdigit(c)) {
            at++;
        }
        if (c < 0) {
            return 0;
        }
        long sign = 1;
        if (c == '-') {
            sign = -1;
            at++;
        }
        long value = 0;
        while ((c = peek_wait(timeout_ms)) >= 0 && isdigit(c)) {
            value = value * 10 + (c - '0');
            at++;
        }
        return sign * value;
    }

  private:
    std::string buffer;
    size_t at = 0;

    size_t emit(const std::string &text)
    {
        fputs(text.c_str(), stdout);
        return text.size();
    }

    static std::string number(unsigned long long v, int base)
    {
        char tmp[72];
        int n = 0;
        do {
            tmp[n++] = "0123456789ABCDEF"[v % (unsigned) base];
            v /= (unsigned) base;
        } while (v);
        std::string out;
        while (n > 0) {
            out += tmp[--n];
        }
        return out;
    }
    static std::string number(long long v, int base, size_t width)
    {
        if (v < 0 && base == 10) {
            return "-" + number((unsigned long long) -v, base);
        }
        if (v < 0) {
            unsigned long long mask = width >= 8 ? ~0ULL : ((1ULL << (width * 8)) - 1);
            return number((unsigned long long) v & mask, base);
        }
        return number((unsigned long long) v, base);
    }
    /* The algorithm of the real Print::printFloat: round by adding half of the
     * last digit, then print the integer part and each decimal in turn. */
    static std::string real(double number_value, int digits)
    {
        if (isnan(number_value)) {
            return "nan";
        }
        if (isinf(number_value)) {
            return "inf";
        }
        if (number_value > 4294967040.0 || number_value < -4294967040.0) {
            return "ovf";
        }
        std::string out;
        if (number_value < 0.0) {
            out += '-';
            number_value = -number_value;
        }
        double rounding = 0.5;
        for (int i = 0; i < digits; i++) {
            rounding /= 10.0;
        }
        number_value += rounding;
        unsigned long whole = (unsigned long) number_value;
        double remainder = number_value - (double) whole;
        out += number((unsigned long long) whole, 10);
        if (digits > 0) {
            out += '.';
        }
        while (digits-- > 0) {
            remainder *= 10.0;
            unsigned int digit = (unsigned int) remainder;
            out += (char) ('0' + digit);
            remainder -= digit;
        }
        return out;
    }

    int timeout_ms = 1000;

    /* The next input byte without taking it, waiting up to `wait_ms` for one
     * (-1 = forever). Returns -1 on timeout. End of input ends the program:
     * a scripted run has nothing more to say, and a real board never sees an
     * end of input. */
    int peek_wait(int wait_ms)
    {
        if (at < buffer.size()) {
            return (unsigned char) buffer[at];
        }
        buffer.clear();
        at = 0;
        struct pollfd pfd = {0, POLLIN, 0};
        fflush(stdout);
        if (poll(&pfd, 1, wait_ms) <= 0) {
            return -1;
        }
        char chunk[256];
        ssize_t got = ::read(0, chunk, sizeof chunk);
        if (got <= 0) {
            sim_quit();
        }
        buffer.assign(chunk, (size_t) got);
        return (unsigned char) buffer[0];
    }
};

inline SimSerial Serial;

void setup();
void loop();

#endif
