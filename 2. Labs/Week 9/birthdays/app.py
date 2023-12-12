import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        #initialize flag to true
        valid = True
        # store & validate user's input for each field
        name = request.form.get("name")
        if not name or not name.isalpha():
            valid = False
        
        month = request.form.get("month")
        if valid and (not month or not month.isdigit() or not (int(month) >= 1 and int(month) <= 12)):
            valid = False
        
        day = request.form.get("day")
        if valid and (not day or not day.isdigit() or not (int(day) >= 1 and int(day) <= 31)):
            valid = False
        
        if valid:
            db.execute("INSERT INTO birthdays(name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")
    else:
        # Retrieves all birthdays from the database and organizes them by name
        birthdays = db.execute("SELECT * FROM birthdays ORDER BY month, day, name")
        # Pass birthdays to index page
        return render_template("index.html", birthdays=birthdays)


# Route for removing the given id's birthday from the database
@app.route("/remove", methods=["POST"])
def remove():
    id = request.form.get("id")
    db.execute("DELETE FROM birthdays WHERE id=?", id)
    return redirect("/")