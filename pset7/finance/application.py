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
    portfolio = db.execute("SELECT * FROM portfolio WHERE id = :id", id = session["user_id"] )
    return render_template("index.html",portfolio = portfolio)

@app.route("/buy", methods=["GET", "POST"])
@login_required
# allows users to buy a certain number of shares of stock
def buy():
    """Buy shares of stock."""
    # add stock to portfolio
    if request.method == "POST":
        # query information that user inputs
        quote = lookup(request.form.get("symbol"))
        share = int(request.form.get("share"))
        if quote:
            if share > 0:
                # user's cash
                money = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"] )

                # can the user afford the stock  SELECT cash FROM users WHERE ...
                if money[0]['cash'] > (quote["price"]*share):
                    # update information of users table
                    db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = money[0]['cash'] - (quote["price"]*share), id = session["user_id"] )
                    # insert new deal into history table
                    db.execute("INSERT INTO history(balance,stock,share,price,id) \
                    VALUES(:balance,:stock,:share,:price,:id)",\
                    balance = money[0]['cash'] - (quote["price"]*share),\
                    stock = request.form.get("symbol"),\
                    share = request.form.get("share"),\
                    price = quote["price"], id = session["user_id"] )

                    share_old = db.execute("SELECT share FROM portfolio WHERE id = :id AND stock = :stock", id = session["user_id"], stock = request.form.get("symbol"))

                    if not share_old:
                        # new stock INSERT
                        db.execute("INSERT INTO portfolio(id,stock,share) \
                        VALUES(:id,:stock,:share)",\
                        id = session["user_id"],\
                        stock = request.form.get("symbol"),\
                        share = request.form.get("share"))

                    else:
                        # same stock UPDATE
                        db.execute("UPDATE portfolio SET share = :share, price = :price, total = :total WHERE id = :id AND stock = :stock",\
                        share = share_old[0]["share"] + int(request.form.get("share")),\
                        id = session["user_id"],\
                        stock = request.form.get("symbol"),\
                        price = quote["price"],\
                        total = quote["price"] * (share_old[0]["share"] + int(request.form.get("share"))))
                    return redirect(url_for("history"))
                else:
                    return apology("NO enough money!")
            else:
                return apology("Share should be positive integer")
        else:
            return apology("Stock is invalid")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    histories = db.execute("SELECT * FROM history WHERE id = :id", id = session["user_id"] )
    return render_template("history.html",histories = histories)

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
            result = db.execute("INSERT INTO users (username,hash) VALUES(:username, :hash)", username = request.form.get("username"), hash = pwd_context.hash(request.form.get("password")))
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
    # add stock to portfolio
    if request.method == "POST":
        # user really has the stock
        money = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"] )
        stock = db.execute("SELECT * FROM portfolio WHERE id = :id AND stock = :stock", id = session["user_id"], stock = request.form.get("symbol") )
        quote = lookup(request.form.get("symbol"))
        share = int(request.form.get("share"))
        share_old = db.execute("SELECT share FROM portfolio WHERE id = :id AND stock = :stock", id = session["user_id"], stock = request.form.get("symbol"))

        if quote:
            if stock:
                if not stock[0]['share'] < share:

                    db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = money[0]['cash'] + (quote["price"]*share), id = session["user_id"] )

                    db.execute("INSERT INTO history(balance,stock,share,price,id) \
                    VALUES(:balance,:stock,:share,:price,:id)",\
                    balance = money[0]['cash'] + (quote["price"]*share),\
                    stock = request.form.get("symbol"),\
                    share = -int(request.form.get("share")),\
                    price = quote["price"], id = session["user_id"] )

                    if stock[0]['share'] > share:
                        db.execute("UPDATE portfolio SET share = :share, price = :price, total = :total WHERE id = :id AND stock = :stock",\
                        share = share_old[0]["share"] - int(request.form.get("share")),\
                        id = session["user_id"],\
                        stock = request.form.get("symbol"),\
                        price = quote["price"],\
                        total = quote["price"] * (share_old[0]["share"] - int(request.form.get("share"))))
                    elif stock[0]['share'] == share:
                        db.execute("DELETE FROM portfolio WHERE stock = :stock",\
                        stock = request.form.get("symbol"))
                    return redirect(url_for("history"))
                else:
                    return apology("not enough share")
            else:
                return apology("stock isn't owned")
        else:
            return apology("stock is invalid")
    else:
        return render_template("sell.html")
