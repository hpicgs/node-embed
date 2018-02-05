console.log('Hi from node-lib-cli.js');

let i = 0;

const hello = () => {
    console.log('Hello again from node-lib-cli.js: ' + i);
    i++;
    if (i < 10) {
        setTimeout(hello, 200);
    }
};

setTimeout(hello, 200);
