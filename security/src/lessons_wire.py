"""Security course, module 2 - plain HTTP is readable on the wire.

Everything here stays on the student's own machine: a login server and a tap
in front of it, both on the loopback interface. The tap keeps the bytes the
client sends, which is what a packet capture of that connection would show,
and it needs no root and nothing installed.
"""

import http.client
import http.server
import os
import socket
import ssl
import tempfile
import threading

import ui

SECRET = "test-password-123"
LOGIN_BODY = f"user=student&password={SECRET}"
TIMEOUT_SECONDS = 5

# A throwaway self-signed pair for "localhost", generated for this lesson and
# published on purpose: it protects nothing and is only ever used on loopback.
# Regenerate with:
#   openssl req -x509 -newkey rsa:2048 -nodes -keyout k.pem -out c.pem
#     -days 36500 -subj /CN=localhost -addext subjectAltName=DNS:localhost
TEST_CERT = """-----BEGIN CERTIFICATE-----
MIIDITCCAgmgAwIBAgIUZySmPvbWrWLfxdh3kjwuMI/9sqkwDQYJKoZIhvcNAQEL
BQAwFDESMBAGA1UEAwwJbG9jYWxob3N0MCAXDTI2MTEwOTEzNTgwM1oYDzIxMjYx
MDE2MTM1ODAzWjAUMRIwEAYDVQQDDAlsb2NhbGhvc3QwggEiMA0GCSqGSIb3DQEB
AQUAA4IBDwAwggEKAoIBAQDaCEbqCSP8BwKOYlOXNsvhqCsN6QGwHMRcOwmVwHLN
g5NKHCKrmifr+bqaqX3BxRYOqcwU+95d/2XN63OmZD9frfj8EXuz2UH/xt775/rX
lPKRT6BqF4Y5+t8QV0mVx13ywJ8SLVvCWKRzRqTcc/q2TWr7bdVSifXWPFaOkDjG
abOYzOS31UovBoGeaJKGvbbcaAwr6ba1OBgjeTxszA5BPSSXosK6U34/mwXtBxRB
7FQicDrHKpjrxmpo/ncj1TjS6aiRTYs9uMJb33g6TZ5iexSaTv9ZgG75Pv4sEjMk
UcZ8MFAFvl9hsbhrIpt6SdpQprLRDc0ZrSIODVPfWGeRAgMBAAGjaTBnMB0GA1Ud
DgQWBBQmdJm5hVJE4po0vZq4IifVtxEjMTAfBgNVHSMEGDAWgBQmdJm5hVJE4po0
vZq4IifVtxEjMTAPBgNVHRMBAf8EBTADAQH/MBQGA1UdEQQNMAuCCWxvY2FsaG9z
dDANBgkqhkiG9w0BAQsFAAOCAQEAb4FJUN1f2VU5B8f5XDnf6wIk37QNVebxeaLs
2QksRdq6GskS2cOKRS8dSCPnnyaB6TpsLSSAGj4NDBVUlPsfDbeEz4acVahA6mRT
/YOlH0qAsh2S8YxKxRSR5WPxkA8nEFn+ehPRu3dLkGQILobpneiW0aAQy/eeLjN/
OqNghzR9yIyXRr+sMa+XOmEdaSlLgLO58Nx5U2PohcMO1kaisjpg2TZxZWcn1C0i
0eML3UL2rcwvwNpj513/VSwPC4lqXj3I0UNl2kVX56DpZlS03TsSnEC7jod197To
S5WNXcraNVpchuuI/fnXWtUPR+ViW6kP1LtgSnxWbDEN1i5iZQ==
-----END CERTIFICATE-----
"""
TEST_KEY = """-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDaCEbqCSP8BwKO
YlOXNsvhqCsN6QGwHMRcOwmVwHLNg5NKHCKrmifr+bqaqX3BxRYOqcwU+95d/2XN
63OmZD9frfj8EXuz2UH/xt775/rXlPKRT6BqF4Y5+t8QV0mVx13ywJ8SLVvCWKRz
RqTcc/q2TWr7bdVSifXWPFaOkDjGabOYzOS31UovBoGeaJKGvbbcaAwr6ba1OBgj
eTxszA5BPSSXosK6U34/mwXtBxRB7FQicDrHKpjrxmpo/ncj1TjS6aiRTYs9uMJb
33g6TZ5iexSaTv9ZgG75Pv4sEjMkUcZ8MFAFvl9hsbhrIpt6SdpQprLRDc0ZrSIO
DVPfWGeRAgMBAAECggEAGeyvBeflU/O/bY+qccntrxywVdyVO0wIWxEYACrcW6xf
6Il04+OWCJxugDJUBiS5jS/KWNpj49hY0gXDgA7ebDGC7/HiPA3Cp7eNTvWT4WtP
9lA5+jNm3eEDAe3nDXyhopodr+sdxHuGvkAxnRbbQi7tYAz8tu45gWo2PohOf+Tc
XSZviMDrgi4ePC8C4m4Qxv57EUTMbMX4Yca9pEzFp0YVh0WkqGTaNiuF5m4v71E5
vhBCI1AbhIqxXleV30iFJbLUZZDhNvTVJx0CtL/8iRk76xjrJAuPg59J74ciKlcd
pjAU1sJbEwi9qbE+gvqhcLRijX8zfbMNkmibNWoiGwKBgQD3ECIJa2wGpf4G8ul7
KDpyd8WcOLqyHaR9xJvr8IRYWKZcvlsCAXPEx5kY8kdiIHAk3dLLLdTvwM6pEg1Z
t6EpN2tdqaoVRsFtQI8OB6881Qp+R/wVPCCjT/wmrmbqPs2jlxWw5Dp5JB2FbuZB
dbSyaKIVzw/v1BSlReG9hvJHYwKBgQDh60/7Tn+xdINIenkX8i9B7V93XcbWaoX7
EAgn/nFmxC5ZrxexR8vxrZCupOdFRnRyLaKJrmJ+wDlyKjcZa0VHGpUa8bcMQIU7
x41k7s48uPXArGjifocVr/gqwrSbwBrUSUuabABAAzTwIiqjPOckFZ9azFot3IAV
+PdZPV3pewKBgQDLEbSYP3ZEJK2eMozcEJ2xtmZzWGGDwz+PrElobYGqR/s9oayW
AnGQXWqmCYXIzta+RqFbJSmvgq+aYd8dTgSmJwoVqz3ETSrxTK4InyJXAtikITC+
l3RQJ48ekBzBDqvheW53va+eIKQvJ48UYNYMClqnSFFWE+CAU+cO69s59wKBgHH6
bhezuuikh7eBUqaqIP157LgkL7TteTr5E6RZ0oyuXVQR6joept3/WwdBWwywRv9z
1pTvzfF0uYbHlZTVcdY1qxQ07szXoGBQhR1u/c3U4ueaFDBw9WYTqXipUsVhXifL
8sCtzFJ/S637DtfcWiDyZ7s/bh/FJznuqZv0QfURAoGAIhitnXPQOOJYYJcjZNfd
0KDyCf66NvY5Um/GTscCoivyiq8ZpidjwgRC6P/nlBeA+sHopv1v/KMXoTcCr56X
xhm1YjHbYJxH38z6vwh68JRHf6n7IaFdhNypArq0CrHANlUrGSUkr6onSIBoQYtf
K3yNh4Q6kew4z1ewW15n/qA=
-----END PRIVATE KEY-----
"""


class _LoginHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        self.rfile.read(int(self.headers.get("Content-Length", 0)))
        body = b"welcome\n"
        self.send_response(200)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, format, *args):
        pass  # the server's own log would land in the middle of the lesson


def _pump(source, destination, record):
    try:
        while data := source.recv(65536):
            if record is not None:
                record.extend(data)
            destination.sendall(data)
    except OSError:
        pass
    finally:
        try:
            destination.shutdown(socket.SHUT_WR)
        except OSError:
            pass


def _login_through_tap(use_tls):
    """Log in once through the tap. Returns (status, bytes the client sent)."""
    server = http.server.ThreadingHTTPServer(("127.0.0.1", 0), _LoginHandler)
    client_context = None

    if use_tls:
        with tempfile.TemporaryDirectory() as folder:
            cert_path = os.path.join(folder, "cert.pem")
            key_path = os.path.join(folder, "key.pem")
            with open(cert_path, "w") as f:
                f.write(TEST_CERT)
            with open(key_path, "w") as f:
                f.write(TEST_KEY)
            server_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
            server_context.load_cert_chain(cert_path, key_path)
        server.socket = server_context.wrap_socket(server.socket, server_side=True)
        client_context = ssl.create_default_context(cadata=TEST_CERT)

    captured = bytearray()
    listener = socket.create_server(("127.0.0.1", 0))

    def tap():
        client, _ = listener.accept()
        upstream = socket.create_connection(server.server_address, TIMEOUT_SECONDS)
        answers = threading.Thread(target=_pump, args=(upstream, client, None), daemon=True)
        answers.start()
        _pump(client, upstream, captured)
        answers.join(TIMEOUT_SECONDS)
        client.close()
        upstream.close()

    threading.Thread(target=server.serve_forever, daemon=True).start()
    tap_thread = threading.Thread(target=tap, daemon=True)
    tap_thread.start()

    port = listener.getsockname()[1]
    if use_tls:
        connection = http.client.HTTPSConnection(
            "localhost", port, timeout=TIMEOUT_SECONDS, context=client_context
        )
    else:
        connection = http.client.HTTPConnection("127.0.0.1", port, timeout=TIMEOUT_SECONDS)

    try:
        connection.request(
            "POST",
            "/login",
            body=LOGIN_BODY,
            headers={"Content-Type": "application/x-www-form-urlencoded"},
        )
        response = connection.getresponse()
        response.read()
        status = response.status
    finally:
        connection.close()
        tap_thread.join(TIMEOUT_SECONDS)
        listener.close()
        server.shutdown()
        server.server_close()

    return status, bytes(captured)


def lesson_02_plain_http():
    ui.title("MODULE 2 - PLAIN HTTP IS READABLE ON THE WIRE")

    ui.heading("PART 1: what anyone on the path receives")

    print("  Your request crosses machines you do not own. Each one receives")
    print("  the same bytes you sent. Whether those bytes mean anything to")
    print("  them is the whole difference between HTTP and HTTPS.")
    print()
    print("  This module starts a login server on your own machine and puts")
    print("  a small tap in front of it, on the loopback interface. The tap")
    print("  forwards the connection and keeps a copy of what the client")
    print("  sent, which is what a packet capture of it would hold.")
    print()
    print("  Your own machine and your own traffic only. Capturing other")
    print("  people's traffic without permission is illegal in most")
    print("  countries, and a school or office network is someone else's.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: the same login over plain HTTP")

    status, captured = _login_through_tap(use_tls=False)
    print(f"  Running: server answered {status}. The tap kept {len(captured)} bytes:")
    print()
    for line in captured.decode("ascii", "replace").split("\r\n"):
        print(f"    {line}")
    print()
    print(f"  Running: password found in the capture? {SECRET.encode() in captured}")
    print()
    print("  No tool had to break anything. The password is in the capture")
    print("  as plain text, because that is what was sent.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: the same login over HTTPS")

    status, captured = _login_through_tap(use_tls=True)
    print(f"  Running: server answered {status}. The tap kept {len(captured)} bytes.")
    print(f"  Running: the first 24 are {captured[:24].hex(' ')}")
    print()
    print(f"  Running: password found in the capture? {SECRET.encode() in captured}")
    print(f"  Running: \"POST /login\" found?         {b'POST /login' in captured}")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 4: the trap -- HTTPS does not hide where you went")

    print(f"  Running: \"localhost\" found in the capture? {b'localhost' in captured}")
    print()
    print("  The server name is sent in the open at the start of the TLS")
    print("  handshake (the SNI field), so the server can pick the right")
    print("  certificate. Anyone watching learns which site you visited,")
    print("  even though the page, the path and the password stay hidden.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(2)

    ui.question(
        "A login form is sent over plain HTTP. Can someone capturing the\n"
        "  traffic read the password?  (answer: yes or no)",
        "yes",
        "PART 2 found it in the capture as plain text.",
    )
    ui.question(
        "Over HTTPS, can someone capturing the traffic read the password?\n"
        "  (answer: yes or no)",
        "no",
        "PART 3 searched the capture for it and it was not there.",
    )
    ui.question(
        "Over HTTPS, can someone capturing the traffic see the server\n"
        "  name?  (answer: yes or no)",
        "yes",
        "It travels in the open in the TLS handshake, as PART 4 found.",
    )

    ui.challenge(
        [
            "You are given one captured request, as a bytes literal:",
            "",
            '  capture = b"POST /login HTTP/1.1\\r\\nHost: shop.test\\r\\n\\r\\nuser=ana&password=abc123"',
            "",
            "Split off the body (it follows the first blank line), find the",
            "password field and print its value after \"leaked: \".",
        ],
        [],
        ["leaked: abc123"],
        [
            'capture = b"POST /login HTTP/1.1\\r\\nHost: shop.test\\r\\n\\r\\nuser=ana&password=abc123"',
            "",
            'body = capture.split(b"\\r\\n\\r\\n")[1]',
            'for pair in body.split(b"&"):',
            '    name, value = pair.split(b"=")',
            '    if name == b"password":',
            '        print("leaked:", value.decode())',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    if not ui.print_summary_pt(2):
        print("   - every machine on the path receives the bytes you send")
        print("   - over plain HTTP those bytes include the password, as text")
        print("   - over HTTPS the capture holds neither the password nor the path")
        print("   - the server name is still visible in the TLS handshake")
        print("   - capture only your own traffic, on your own machine")
    ui.wait_enter()
