from flask import Flask, request, jsonify

app = Flask(__name__)


@app.route("/temperature", methods=["POST"])
def receive_temperature():
    data = request.json
    print("Received data:", data)
    return "", 200


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=3000)
