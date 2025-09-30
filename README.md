<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>LocalCat</title>
</head>
<body>
    <h1>LocalCat 🐱💬</h1>
    <p><strong>LocalCat</strong> is a lightweight local chat application written in C++. It can be used as a standalone chat app or integrated as a library in your own projects.</p>

    <h2>Features</h2>
    <ul>
        <li>Easy-to-use local chat system in C++</li>
        <li>Supports multiple clients</li>
        <li>Broadcast messages to all clients or send to a specific client</li>
        <li>Can be used as a library in your own C++ projects</li>
    </ul>

    <h2>Getting Started</h2>

    <h3>Server</h3>
    <ol>
        <li>Place <em>src/server.cpp</em> in your project.</li>
        <li>Create a Server object, specifying an IP and port, or use the default local settings.</li>
        <li>Run the server.</li>
        <li>Send messages to all clients or a specific client.</li>
    </ol>

    <h3>Client</h3>
    <ol>
        <li>Place <em>src/client.cpp</em> in your project.</li>
        <li>Create a Client object.</li>
        <li>Connect the client to the server using IP and port.</li>
        <li>Send and receive messages.</li>
    </ol>

    <h2>Contributing</h2>
    <p>Feel free to fork the repository and submit pull requests. Contributions, issues, and suggestions are welcome!</p>

    <h2>License</h2>
    <p>This project is open-source. Feel free to use it in your projects.</p>
</body>
</html>
