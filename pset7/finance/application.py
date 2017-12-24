from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
# user must be logged in order to access the given route
@login_required
def index():
    return render_template("Uindex.html")
    # return apology("TODO")

@app.route("/buy", methods=["GET", "POST"])
@login_required
# allows users to buy a certain number of shares of stock
def buy():
    """Buy shares of stock."""
    # add stock to portfolio
    if request.method == "POST":
        # query information that user inputs
        stock = request.form.get("symbol")
        price = lookup(request.form.get("symbol"))
        share = int(request.form.get("share"))
        if price:
            if share > 0:
                # user's cash
                cash = db.execute("SELECT * FROM users WHERE id = :id", id = session["user_id"] )
            else:
                apology("Share should be positive integer")
        else:
            apology("Stock is invalid")

        # can the user afford the stock  SELECT cash FROM users WHERE id=1

        # buying more of the same stock  INSERT INTO ...

        ##!!! new SQL table for

    # update cash
        # UPDATE users SET cash = cash - 50 WHERE id = 1
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

# login, already implemented
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        # db.execute always return an array
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


# logout, already implemented
@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

# login requirement
@app.route("/quote", methods=["GET", "POST"])
@login_required
# allows users to look up the price of stocks
def quote():
    """Get stock quote."""
    quoted = False
    if request.method == "POST":
        # retrieve stock quote
        quote = lookup(request.form.get("symbol"))
        if quote :
            #display stock quote, in HTML
            return render_template("quoted.html", stock = quote)
        else :
            apology("Stock is invalid")



    # ensure the stock is valid, or apology
    else:
        return render_template("quote.html")
@app.route("/register", methods=["GET", "POST"])
# user can sign up for the website, implemented
def register():
    """Register user."""
# display form(in HTML!!)
    if request.method == "POST":
# valid passwords?
        # ensure fields aren't left blank(else, apologize)
        if (request.form.get("username") == ""):
            return apology("Missing username")
        elif (request.form.get("password") == ""):
            return apology("Missing password")
        # ensure password and confirmation field match(else, apologize)
        elif(request.form.get("confirmation") != request.form.get("password")):
            return apology("unmatch password")
        else:
# add user to database
            # store encypted password by pwd_context.encrypt
            result = db.execute("INSERT INTO users (username,hash) VALUES(:username, :hash)", username = request.form.get("username"), hash = pwd_context.encrypt(request.form.get("password")))
            if not result:
                return apology("username is already existed")
            # if not result: return apology("")
# once they register sucessfully, log them in automatically
            # store thier id in session, session ["user_id"]
            session["user_id"] = result
            return redirect(url_for("index"))
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    return apology("TODO")
