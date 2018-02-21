
let i = 1;

const demoEvent = () => {
    console.log('[JS ] This is the ' + i + '. event.');
    i++;
    if (i < 4) {
        setTimeout(demoEvent, 200);
    }
};

setTimeout(demoEvent, 200);

console.log('[JS ] node-lib-cli.js was executed and events were scheduled.');
