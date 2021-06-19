
#ifndef HTML_H
#define HTML_H

#include <string>

#define HTML(...) #__VA_ARGS__

inline void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}


const std::string HTML_BODY_START = HTML(
<!doctype html>
<html lang="en-US">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>FTX Pay Me Configurator</title>
        <link rel="stylesheet" href="https://cdn.simplecss.org/simple.min.css">
    </head>
    <body>
    <header>
        <h1>FTX Pay Me Configurator</h1>
        <p>Gimmie that money, bitch</p>
    </header>
);

const std::string HTML_CONFIRMATION = HTML(
    <p>
        <p><strong>Configuration saved</strong></p>
        <p><a href="/">back</a></p>
    </p>
);

const std::string HTML_BODY_END = HTML(
        <footer>
            <p>FTX Pay Me Configurator.</p>
        </footer>
    </body>
</html>
);

const std::string HTML_FORM(int fttTokens, bool btc, bool eth) {
    std::string ret = HTML(
        <form action="/save" method="POST">
            <p><strong>Fill out the form below to configure</strong></p>
            <p>
                <label>How many FTT tokens are you awarded per month?</label><br>
                <input type="number" name="fttTokens" value="{fttTokens}">
            </p>
            <p>
                <label>Include additional coiints</label> <br />
                <input type="checkbox" name="btc" value="true" {btc} /> BTC <br />
                <input type="checkbox" name="eth" value="true" {eth}/ > ETH <br />
            </p>
            <p>
                <label>Anything else?</label><br />
                <input type="text" name="message" value="(___)__)=======D">
            </p>
            <button type="submit">Save</button>
        </form>
    );
    replaceAll(ret, "{fttTokens}", std::to_string(fttTokens));
    replaceAll(ret, "{btc}", btc ? "checked" : "");
    replaceAll(ret, "{eth}", eth ? "checked" : "");
    return ret;
}

#endif
