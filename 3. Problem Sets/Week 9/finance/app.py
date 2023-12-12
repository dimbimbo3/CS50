import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from re import fullmatch # for checking password requirements

from helpers import apology, login_required, lookup, usd, datetime # for transaction timestamp

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    grandTotal = 0 # initialize running grand total
    stocks = [] # initialize empty list for each stock

    # retrieve the user's current cash on hand
    user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = float(user[0]["cash"])
    # add cash to running total
    grandTotal += cash

    # retrieve all stocks owned by user
    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ?", session["user_id"])

    # loops through each of the user's stocks
    for row in portfolio:
        stock = lookup(row["symbol"]) # retrieves current price of each stock's shares
        sTotal = float(stock["price"]) * int(row["shares"]) # calculates the total of each stock
        grandTotal += sTotal # adds stock total to running grand total
        stocks.append({"symbol" : stock["symbol"], "name" : stock["name"], "shares" : row["shares"], "price" : usd(stock["price"]), "total" : usd(sTotal)})

    # pass variables to index
    return render_template("index.html", stocks=stocks, cash=usd(cash), grandTotal=usd(grandTotal))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached this route via POST (trying to buy shares)
    if request.method == "POST":
        # Retrieve & validate user input
        # SYMBOL
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("symbol missing", 400)
        
        # SHARES
        shares = request.form.get("shares")
        if not shares or int(shares) < 1:
            return apology("shares must be a postive number", 400)
        
        # Retrieve & validate stock information
        stock = lookup(symbol)
        if not stock:
            return apology("invalid symbol", 400)
        
        # Check if user has enough cash to buy the shares
        user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = float(user[0]["cash"])
        cost = float(stock["price"]) * int(shares)
        if cost > cash:
            return apology("cannot afford shares", 400)
        else:
            cash -= cost # update cash to reflect purchase of shares

        # Check for existing shares in user's portfolio
        portfolio = db.execute("SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], stock["symbol"])
        if len(portfolio) != 1:
            # this user does not already have shares of this stock
            db.execute("INSERT INTO portfolio(user_id, symbol, shares) VALUES(?, ?, ?)", session["user_id"], stock["symbol"], shares)
        else:
            # this user has shares of this stock already
            sharesTotal = int(shares) + int(portfolio[0]["shares"])
            db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?", sharesTotal, session["user_id"], stock["symbol"])

        # Commit purchase of shares
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"]) # Update user's cash supply
        time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") # get time of transaction (without microseconds)
        db.execute("INSERT INTO transactions(user_id, time, symbol, shares, price) VALUES(?, ?, ?, ?, ?)", session["user_id"], time, stock["symbol"], shares, stock["price"]) # record transaction

        # alert user and redirect to index route
        flash("Shares purchased.")
        return redirect("/")
    
    # User reached this route via GET
    else:
        return render_template("buy.html") # direct user to buy form


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Retrieve user's transaction history
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY time", session["user_id"])
    # Change all transaction prices to usd format
    for transaction in transactions:
        transaction["price"] = usd(transaction["price"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id, but keep any flash messages
    if session.get("_flashes"):
        flashes = session.get("_flashes")
        session.clear()
        session["_flashes"] = flashes
    else:
        session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Logged in.")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # flash logout message
    flash("Logged out.")

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached this route via POST (trying to quote a symbol)
    if request.method == "POST":
        # Retrieve & validate user input
        # SYMBOL
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("symbol missing", 400)
        
        # Retrieve & validate stock information
        stock = lookup(symbol)
        if not stock:
            return apology("invalid symbol", 400)

        # Change price to usd format and pass dictionary to page
        stock["price"] = usd(stock["price"])
        return render_template("quoted.html", stock=stock)

    # User reached this route via GET
    else:
        return render_template("quote.html") # direct user to stock quote form


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached this route via POST (trying to create a new account)
    if request.method == "POST":
        # Retrieve & validate user input
        # USER
        username = request.form.get("username")
        if not username:
            return apology("no username was entered", 400)
        
        # PASS
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not password or not confirmation:
            return apology("password missing", 400)
        elif password != confirmation:
            return apology("passwords do not match", 400)
        
        # Ensure password meets requirements (at least 8 characters, 1 upperlower, 1 uppercase, 1 number, 1 symbol)
        if not fullmatch(r"(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[!@#$%^&*]).{8,}", password):
            return apology("invalid password", 400)
        
        # Check for existing username
        query = db.execute("SELECT username FROM users WHERE username = ?", username)
        if len(query) > 0:
            return apology("username already in use", 400)
        
        # hash the given password
        hashed = generate_password_hash(f"{password}")
        # add new user to database
        db.execute("INSERT INTO users(username, hash) VALUES(?, ?)", username, hashed)
        # log newly created user in
        user = db.execute("SELECT id FROM users WHERE username = ?", username)
        id = user[0]["id"]
        session["user_id"] = id
        # alert user and redirect to index route
        flash("Account successfully created.")
        return redirect("/")

    # User reached this route via GET
    else:
        return render_template("register.html") # direct user to registration form


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached this route via POST (trying to sell shares)
    if request.method == "POST":
        # Retrieve & validate user input
        # SYMBOL
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)
        
        # SHARES
        shares = request.form.get("shares")
        if not shares or int(shares) < 1:
            return apology("shares must be a postive number", 400)
        
        # Retrieve current price of stock
        stock = lookup(symbol)

        # Retrieve user's total of shares
        portfolio = db.execute("SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], stock["symbol"])
        total = int(portfolio[0]["shares"])
        # Check if user is trying to sell more shares than they have
        if int(shares) > total:
            return apology("too many shares", 400)
        
        # Commit sale of shares
        nShares = int(shares) * -1 # negative value indicates sale
        total += nShares # update current total of shares
        # Check if all shares of the given stock were sold
        if total == 0:
            db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], stock["symbol"])
        else:
            db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?", total, session["user_id"], stock["symbol"])

        # Retrieve user's current cash
        user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = float(user[0]["cash"])
        # Add sold shares to cash
        sold = float(stock["price"]) * int(shares)
        cash += sold

        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"]) # Update user's cash supply
        time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") # get time of transaction (without microseconds)
        db.execute("INSERT INTO transactions(user_id, time, symbol, shares, price) VALUES(?, ?, ?, ?, ?)", session["user_id"], time, stock["symbol"], nShares, stock["price"]) # record transaction

        # alert user and redirect to index route
        flash("Shares sold.")
        return redirect("/")

    # User reached this route via GET
    else:
        # Retrieve the user's current stocks 
        portfolio = db.execute("SELECT symbol FROM portfolio WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", stocks=portfolio) # direct user to sell form