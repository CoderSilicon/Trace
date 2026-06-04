<div
    class="flex min-h-screen flex-col bg-[#0a0a0a] px-6 py-24 jetbrains-mono-400 text-[#8e929a] selection:bg-[#00f5c4]/30"
>
    <main class="relative z-10 mx-auto w-full max-w-3xl">
        <!-- Mock Terminal Top Bar (Hard Edges Only) -->
        <div class="mb-16 flex items-center justify-between border-b border-white/5 pb-4">
            <div class="flex gap-2">
                <span class="h-2.5 w-2.5 border border-white/10 bg-transparent"></span>
                <span class="h-2.5 w-2.5 border border-white/10 bg-transparent"></span>
                <span class="h-2.5 w-2.5 border border-white/10 bg-transparent"></span>
            </div>
            <span class="text-xs text-[#545861] select-none">~/about/the-author.md</span>
        </div>

        <!-- Header -->
        <header class="mb-12">
            <h1 class="jetbrains-mono-700 text-3xl leading-tight tracking-tight text-white md:text-4xl">
                I didn't set out to <br />
                <span class="text-[#00f5c4]">build a CLI.</span>
            </h1>
        </header>

        <!-- Markdown Style Body Content -->
        <article class="space-y-6 text-sm leading-relaxed md:text-[0.95rem]">
            <p>
                For years, my workflow was fragmented. I just wanted to pull specific data from a website,
                but I always ended up chaining <code class="bg-white/5 px-1.5 py-0.5 text-white">curl</code>, 
                <code class="bg-white/5 px-1.5 py-0.5 text-white">grep</code>,
                <code class="bg-white/5 px-1.5 py-0.5 text-white">sed</code>, and
                <code class="bg-white/5 px-1.5 py-0.5 text-white">jq</code>. It
                worked, but it was incredibly brittle—every time a website changed its markup even slightly,
                my entire pipeline broke.
            </p>

            <p>
                As my focus shifted deep into systems engineering, I realized my tools weren't keeping up.
                The solution was actually hiding in my own archives: an old, abandoned web-based link
                shortener called <span class="text-white">LinkBytes</span> that I had built as an early learning project. It had sat
                gathering dust for years.
            </p>

            <p>
                I stripped away the web interface, completely redesigned the core engine, and forged it
                into a lightning-fast tool built entirely for the command line. I built this because the
                best tools feel painfully obvious in retrospect; it is the exact, frictionless tool I wish
                I had five years ago.
            </p>
        </article>
    </main>

    <!-- Terminal Footer Signature -->
    <div class="mt-24 flex items-center justify-center">
        <span class="jetbrains-mono-700 animate-pulse text-lg text-[#00f5c4]">_</span>
    </div>
</div>