import sqlite3
from typing import Any

import requests

URL: str = "https://api.nasdaq.com/api/screener/stocks?tableonly=true&offset=0&download=True"
HEADERS: dict[str, str] = {
	"User-Agent": "Mozilla/5.0",
	"Accept": "application/json, text/plain, */*",
}

def getNasdaqSymbols() -> dict[str, Any] | None:
	response = requests.get(URL, headers=HEADERS, timeout=15)
	if response.status_code == 200:
		return response.json()
	return None

def sendNasdaqSymbolsToDb(symbols: list[dict[str, Any]]) -> None:
	con = sqlite3.connect("master.db")
	cur = con.cursor()
	
	cur.execute("CREATE TABLE IF NOT EXISTS symbol("
	                "id INTEGER PRIMARY KEY, "
			"ticker TEXT, "
			"country TEXT, "
			"sector TEXT, "
			"name TEXT, "
			"exchange TEXT)")

	stmt = "INSERT INTO symbol VALUES(?, ?, ?, ?, ?, ?)"

	#TODO: make string sanitizer because some tickers(symbol) have non-alpha chars
	for i in range(len(symbols)):
		cur.execute(stmt, (None,
				symbols[i]['symbol'],
				symbols[i]['country'],
				symbols[i]['sector'],
				symbols[i]['name'],
				"NASDAQ"))

	con.commit()
	con.close()


if __name__ == "__main__":
	nasdaqSymbols = getNasdaqSymbols()
	if nasdaqSymbols is not None:
		print(len(nasdaqSymbols['data']['rows']))
		sendNasdaqSymbolsToDb(nasdaqSymbols['data']['rows'])
	print("Finished...")
		
