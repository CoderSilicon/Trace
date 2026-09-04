![sget](./assets/logo.png)

# sget

**High-performance CLI data-tracer and metadata extractor.**

`sget` is a unique blend of modern ricing and infoation tech. It allows you to inspect a website with simultaenously also able to download files like `curl` and also operates directly on raw HTTP streams. Built in native C++ on top of `libcurl`, it treats the web as a queryable data pipeline with zero runtime overhead and minimal memory consumption.

[Architecture](#-architecture) • [Key Features](#-features) • [Quickstart](#-quickstart) • [Usage](#-usage)


## ⚡ Features

* **Native `libcurl` Core:** Low-latency HTTP transfers, custom TLS handshakes, proxy chains, and cookie handling.
* **Stream Extraction:** Zero-copy parsing for CSS selectors, XPath nodes, and metadata boundaries on raw streams.
* **Unix-Pipeline Native:** Emits clean, structured data to `stdout` designed to pipe directly into `jq`, `grep`, or local storage.
* **Low Footprint:** No headless browser engines or heavy JS evaluation loops—pure C++ execution.


## 🏗️ Architecture

```text
[ Target Endpoint ]
        │
        ▼  (Low-Level Stream Fetch)
  ┌───────────┐
  │  libcurl  │
  └───────────┘
        │
        ▼  (Raw Memory Buffer)
  ┌───────────┐
  │   sget    │ ──► [ Extraction Engine: CSS / XPath / Metadata ]
  └───────────┘
        │
        ▼  (Flushed Output)
  [ JSON / Raw ] ──► Pipe to terminal stdout / file

```



## 🛠️ Tech Stack

| Layer | Technology | Purpose |
| --- | --- | --- |
| **Network Engine** | `libcurl` | High-throughput HTTP/S transfer & socket management |
| **Language** | C++17 | Direct memory management and low-latency execution |
| **Package Manager** | `vcpkg` | C++ dependency resolution |
| **Environment** | Docker / Native | Standalone cross-platform distribution |



## 🚀 Quickstart

### Building from Source

```bash
# Clone the repository
git clone [https://github.com/CoderSilicon/sget.git](https://github.com/CoderSilicon/sget.git)
cd sget

# Compile using Makefile
make

# Run sget
./bin/sget --help

```

## 💻 Basic Usage

```bash
# Extract open-graph metadata as JSON
sget [https://news.ycombinator.com](https://news.ycombinator.com) --json

# Extract specific CSS selectors and pipe to jq
sget [https://example.com](https://example.com) --select "h1.title" | jq .

# Run through a proxy with custom headers
sget [https://api.example.com/data](https://api.example.com/data) --proxy "socks5://127.0.0.1:9050" -H "User-Agent: sget/1.0"

```


 <br />


<div align="center">

<sub>Built with 💻 by <a href="https://github.com/CoderSilicon">CoderSilicon</a></sub>

<br />

<em>"It is always better to differ from others."</em>

</div> 