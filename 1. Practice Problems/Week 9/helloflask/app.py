from flask import Flask, render_template, request

app = Flask(__name__) # uses current script's filename

COLORS = ['red', 'blue'] # valid color choices

@app.route("/", methods=["GET","POST"])
def index():
    # send to form on GET request
    if request.method == "GET":
        return render_template("index.html")
    # else this is a POST request from a form
    else:
        color = request.form.get("color") # stores selected value in variable
        # validates user's selection
        if color in COLORS:
            print("Form submitted!")
            return render_template("color.html", color=color) # passes color variable as "color" to the template "color.html"
        else:
            print("Form NOT submitted!")
            return render_template("index.html")
