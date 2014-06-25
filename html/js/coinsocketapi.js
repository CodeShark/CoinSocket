var connectbutton; // = document.getElementById('connectbutton');
var output; // = document.getElementById('output');
var autoscroll; // = document.getElementById('autoscroll');
var statusbar; // = document.getElementById('statusbar');

var urlinput; // = document.getElementById('url');
var serverurl;

var uridiv;

var ws = null;

var requestid = 0;

var onopen = function () {
    setstatus('Connected to ' + serverurl + '.');
    appendoutput('CONNECTED TO ' + serverurl + '.');
    connectbutton.value = 'Disconnect';
};

var onclose = function () {
    ws = null;
    setstatus('Connection closed.');
    appendoutput('DISCONNECTED FROM ' + serverurl + '.');
    connectbutton.value = 'Connect';
};

var onmessage = function (e) {
    console.log(e.data);
    var obj = JSON.parse(e.data);
    var json = JSON.stringify(obj, undefined, 2);
    output.value += json + '\n\n';
    if (autoscroll.checked)
        output.scrollTop = output.scrollHeight;

    // update uri
    if (obj.hasOwnProperty('result') && obj.result.hasOwnProperty('uri')) {
        uridiv.innerHTML = '<a href="' + obj.result.uri + '">Request from local wallet...</a>';
    }
    else {
        uridiv.innerHTML = '';
    }
};

function connect() {
    if (ws) {
        // enablesubmit(false);
        ws.close();
        return;
    }

    serverurl = urlinput.value;
    setstatus('Connecting to ' + serverurl + '...');

    ws = new WebSocket(serverurl);
    ws.onopen = onopen;
    ws.onclose = onclose;
    ws.onmessage = onmessage;
    // enablesubmit(true); 
}

function sendrequest(req) {
    if (!ws) return;

    appendoutput('Sending ' + req  + ' ...');
    if (autoscroll.checked)
        output.scrollTop = output.scrollHeight;

    ws.send(req);
}

function appendoutput(o) {
    output.value += o + '\n\n';
}

function setstatus(s) {
    statusbar.innerHTML = s;
}

window.onload = function () {
    connectbutton = document.getElementById('connectbutton');
    output = document.getElementById('output');
    autoscroll = document.getElementById('autoscroll');
    statusbar = document.getElementById('statusbar');
    urlinput = document.getElementById('url');
    uridiv = document.getElementById('uridiv');
};

// COMMANDS

// Global Operations
function getvaultinfo() {
    if (!ws) return;
    var req = '{"method": "getvaultinfo", "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

// Keychain Operations
function getkeychains() {
    if (!ws) return;
    var req = '{"method": "getkeychains", "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

function getkeychaininfo() {
    if (!ws) return;
    var keychainname = document.getElementById('getkeychaininfo_name').value;
    var req = '{"method": "getkeychaininfo", "params":["' + keychainname + '"], "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

function newkeychain() {
    if (!ws) return;
    var keychainname = document.getElementById('newkeychain_name').value;
    var req = '{"method": "newkeychain", "params":["' + keychainname + '"], "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

function renamekeychain() {
    if (!ws) return;
    var oldname = document.getElementById('renamekeychain_oldname').value;
    var newname = document.getElementById('renamekeychain_newname').value;
    var req = '{"method": "renamekeychain", "params":["' + oldname + '", "' + newname + '"], "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

// Account Operations
function getaccounts() {
    if (!ws) return;
    var req = '{"method": "getaccounts", "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

function getaccountinfo() {
    if (!ws) return;
    var accountname = document.getElementById('getaccountinfo_name').value;
    var req = '{"method": "getaccountinfo", "params": ["' + accountname + '"], "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

// Blockchain Operations
function getchaintip() {
    if (!ws) return;
    var req = '{"method": "getchaintip", "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

function getblockheader_int() {
    if (!ws) return;
    var height = document.getElementById('getblockheader_int_height').value;
    var req = '{"method": "getblockheader", "params":[' + height + '], "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}

function getblockheader_string() {
    if (!ws) return;
    var hash = document.getElementById('getblockheader_string_hash').value;
    var req = '{"method": "getblockheader", "params":["' + hash + '"], "id": ' + requestid + '}';
    requestid++;
    sendrequest(req);
}