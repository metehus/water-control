//
// Created by methi on 31/01/2024.
//

#ifndef NETWORK_HTML_H
#define NETWORK_HTML_H

const String NETWORK_AP_HTML PROGMEM = R"""(
<title>Bombinha WiFi Setup</title><meta name="viewport" content="width=device-width, initial-scale=1.0"><main><div style=max-width:20rem;width:100%;display:flex;flex-direction:column;gap:2rem><div><h2 style=margin-top:1.5rem;text-align:center;font-size:1.875rem;font-weight:800>Connect to WiFi</h2></div><form style=margin-top:2rem;display:flex;flex-direction:column;gap:1.5rem><div style="border-radius:.375rem;box-shadow:#0000000d 0 1px 2px 0;display:flex;flex-direction:column;gap:1rem"><div><label for=ssid>WiFi SSID</label> <input id=ssid placeholder="Enter ssid"required></div><div><label for=password>Password</label> <input id=password placeholder="Enter password"required type=password></div></div><div><button type=submit>Connect</button></div></form></div><style>body,html{margin:0;font-family:'Segoe UI Semibold',sans-serif;min-height:100dvh;background-color:#f7fafc;display:flex;align-items:center;justify-content:center;flex-direction:column}label{color:#4a5568}input{margin-top:4px;border-radius:.375rem;display:block;width:100%;padding:.5rem .75rem;border:1px solid #d2d6dc;color:#1a202c;font-size:.875rem}button{width:100%;padding:.5rem 1rem;border:none;cursor:pointer;font-size:1rem;font-weight:500;border-radius:.375rem;color:#fff;background-color:#667EEAFF}</style>
<script defer>
    document.querySelector("form").onsubmit=e=>{e.preventDefault(),document.querySelector("button").innerHTML='<svg style="height:18px" viewBox="0 0 100 100"><path fill="#fff" d="M73,50c0-12.7-10.3-23-23-23S27,37.3,27,50 M30.9,50c0-10.5,8.5-19.1,19.1-19.1S69.1,39.5,69.1,50"><animateTransform attributeName="transform" attributeType="XML" type="rotate"dur="1s" from="0 50 50"to="360 50 50" repeatCount="indefinite" /></path></svg>',fetch("/connect",{method:"POST",body:new URLSearchParams({ssid:document.querySelector("#ssid").value,password:document.querySelector("#password").value})})};
</script></main>
)""";

#endif //NETWORK_HTML_H

