<script lang="ts">
	// State for active view and mobile menu
	let currentView = 'Introduction';
	let isMobileMenuOpen = false;

	// Navigation structure
	const navSections = [
		{
			title: 'GETTING STARTED',
			links: ['Introduction', 'Installation', 'Quick Start']
		},
		{
			title: 'CORE CONCEPTS',
			links: ['Basic Usage', 'Selectors', 'Output Formats']
		},
		{
			title: 'ADVANCED',
			links: ['Interactive Mode', 'Sessions & Auth', 'Watch Mode', 'Configuration']
		},
		{
			title: 'REFERENCE',
			links: ['CLI Reference', 'Examples']
		}
	];

	// Flatten links for dynamic Next/Prev pagination
	const allLinks = navSections.flatMap((s) => s.links);
	$: currentIndex = allLinks.indexOf(currentView);
	$: prevPage = currentIndex > 0 ? allLinks[currentIndex - 1] : null;
	$: nextPage = currentIndex < allLinks.length - 1 ? allLinks[currentIndex + 1] : null;

	// Features list for Intro
	const features = [
		'CSS and XPath selectors for precise element targeting',
		'JSON, CSV, and table output formats',
		'Interactive TUI for exploring pages',
		'Session management with cookie persistence',
		'Watch mode for monitoring changes',
		'Built-in rate limiting and retry logic',
		'Single binary, zero dependencies'
	];

	function setView(view: string) {
		currentView = view;
		isMobileMenuOpen = false; // Close mobile menu on navigate
		window.scrollTo({ top: 0, behavior: 'smooth' }); // Scroll to top
	}

	function toggleMenu() {
		isMobileMenuOpen = !isMobileMenuOpen;
	}
</script>

<div
	class=" relative flex min-h-screen flex-col bg-[#0a0a0a] text-white selection:bg-[#00f5c4]/30 md:flex-row"
>
	<div
		class="sticky top-0 z-30 flex items-center justify-between border-b border-white/5 bg-[#0a0a0a] p-4 md:hidden"
	>
		<div class="jetbrains-mono-700 text-lg tracking-tight">
			<span class="mr-0.5 text-[#00f5c4]"> docs</span>
		</div>
		<button
			onclick={toggleMenu}
			class="jetbrains-mono-400 text-xs tracking-widest text-[#8e929a] transition-colors hover:text-white focus:outline-none"
		>
			{isMobileMenuOpen ? '[ CLOSE ]' : '[ MENU ]'}
		</button>
	</div>

	{#if isMobileMenuOpen}
		<div class="fixed inset-0 z-20 md:hidden" onclick={toggleMenu}></div>
	{/if}

	<aside
		class="
         fixed inset-y-0 bottom-0 left-0 z-50 h-full w-64 shrink-0 overflow-y-auto border-r border-white/5 bg-[#0a0a0a] p-6 transition-transform duration-300 ease-in-out md:sticky md:top-0 md:block md:w-72 md:translate-x-0 md:p-8
        {isMobileMenuOpen ? 'translate-x-0' : '-translate-x-full'}
    "
	>
		<div class="jetbrains-mono-700 mb-10 hidden text-lg tracking-tight md:block">docs</div>

		<nav class="space-y-10">
			{#each navSections as section (section)}
				<div>
					<h3
						class="jetbrains-mono-600 mb-4 text-[0.65rem] tracking-widest text-[#545861] uppercase select-none"
					>
						{section.title}
					</h3>
					<ul class="space-y-3 pl-2">
						{#each section.links as link (link)}
							<li class="relative">
								{#if currentView === link}
									<div class="absolute top-0 bottom-0 -left-2 w-0.5 bg-[#00f5c4]"></div>
								{/if}
								<button
									onclick={() => setView(link)}
									class="jetbrains-mono-400 block w-full text-left text-sm transition-colors duration-200 {currentView ===
									link
										? 'text-[#00f5c4]'
										: 'text-[#8e929a] hover:text-white'}"
								>
									{link}
								</button>
							</li>
						{/each}
					</ul>
				</div>
			{/each}
		</nav>
	</aside>

	<main class="min-w-0 flex-1 px-6 py-10 md:px-12 md:py-16 lg:px-20">
		<div class="max-w-3xl">
			<div
				class="jetbrains-mono-400 mb-8 flex items-center gap-2 text-xs text-[#545861] select-none"
			>
				<span>Docs</span>
				<span>&rsaquo;</span>
				<span class="text-[#8e929a]">{currentView}</span>
			</div>

			<h1 class="jetbrains-mono-700 mb-8 text-3xl tracking-tight text-white md:text-4xl">
				{currentView}
			</h1>

			{#if currentView === 'Introduction'}
				<p
					class="jetbrains-mono-400 mb-10 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]"
				>
					sget is a command-line tool for extracting data from websites. Think of it as curl with
					superpowers: CSS selectors, structured output, interactive mode, and visual diffs, all in
					a single binary.
				</p>

				<h2 class="jetbrains-mono-700 mb-4 text-xl text-white">Why sget?</h2>
				<p class="jetbrains-mono-400 mb-4 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					If you've ever written a command like this:
				</p>

				<div class="mb-6 overflow-hidden border border-white/10 bg-[#0f0f0f] shadow-lg">
					<div
						class="flex items-center justify-between border-b border-white/5 bg-white/2 px-4 py-2"
					>
						<span class="jetbrains-mono-400 text-xs text-[#545861]">bash</span>
					</div>
					<div class="jetbrains-mono-400 overflow-x-auto p-4 text-sm whitespace-nowrap">
						<span class="text-[#00f5c4]">curl</span> <span class="text-white">-s</span>
						https://example.com | <span class="text-[#00f5c4]">grep</span>
						<span class="text-[#ffbd2e]">'&lt;title&gt;'</span>
						| <span class="text-[#00f5c4]">sed</span>
						<span class="text-[#ffbd2e]">'s/&lt;[^&gt;]*&gt;//g'</span>
					</div>
				</div>

				<p class="jetbrains-mono-400 mb-4 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					Then sget is for you. The same thing with sget:
				</p>

				<div class="mb-10 overflow-hidden border border-white/10 bg-[#0f0f0f] shadow-lg">
					<div
						class="flex items-center justify-between border-b border-white/5 bg-white/2 px-4 py-2"
					>
						<span class="jetbrains-mono-400 text-xs text-[#545861]">bash</span>
					</div>
					<div class="jetbrains-mono-400 overflow-x-auto p-4 text-sm whitespace-nowrap">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">https://example.com --select</span>
						<span class="text-[#ffbd2e]">"title"</span> <span class="text-white">--text</span>
					</div>
				</div>

				<h2 class="jetbrains-mono-700 mb-5 text-xl text-white">Features</h2>
				<ul class="mb-16 space-y-3.5">
					{#each features as feature (feature)}
						<li class="flex items-start">
							<span class="mt-2 mr-3.5 inline-block h-1.5 w-1.5 shrink-0 bg-[#00f5c4]"></span>
							<span
								class="jetbrains-mono-400 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]"
								>{feature}</span
							>
						</li>
					{/each}
				</ul>
			{:else if currentView === 'Installation'}
				<p class="jetbrains-mono-400 mb-8 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					sget is distributed as a single standalone binary. You can install it using your favorite
					package manager or download it directly.
				</p>

				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">macOS / Linux (Homebrew)</h2>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-white">brew install sget-cli</span>
					</div>
				</div>

				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">Windows (Scoop)</h2>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-white">scoop install sget</span>
					</div>
				</div>

				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">From Source (Cargo)</h2>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-white">cargo install sget-cli</span>
					</div>
				</div>
			{:else if currentView === 'Quick Start'}
				<p class="jetbrains-mono-400 mb-6 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					Let's extract all the headline links from Hacker News in one simple command.
				</p>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 overflow-x-auto p-4 text-sm whitespace-nowrap">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">https://news.ycombinator.com -s</span>
						<span class="text-[#ffbd2e]">".titleline > a"</span>
						<span class="text-white">--attr href</span>
					</div>
				</div>
				<p class="jetbrains-mono-400 mb-8 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					This command fetches the page, parses the DOM, finds all anchor tags inside elements with
					the class <code>titleline</code>, and prints out their <code>href</code> attributes.
				</p>
			{:else if currentView === 'Basic Usage'}
				<p class="jetbrains-mono-400 mb-6 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					The basic syntax of sget is designed to be intuitive. You provide a URL and tell sget what
					you want to extract.
				</p>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm whitespace-nowrap">
						<span class="text-[#545861]"># General Syntax</span><br />
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">[URL] [OPTIONS]</span>
					</div>
				</div>

				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">Sending Headers</h2>
				<p class="jetbrains-mono-400 mb-4 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					Pass headers using the <code>-H</code> flag, exactly like curl:
				</p>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">https://api.github.com -H</span>
						<span class="text-[#ffbd2e]">"Authorization: Bearer TOKEN"</span>
					</div>
				</div>
			{:else if currentView === 'Selectors'}
				<p class="jetbrains-mono-400 mb-6 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					sget supports both CSS and XPath selectors natively.
				</p>

				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">CSS Selectors</h2>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">https://site.com --select</span>
						<span class="text-[#ffbd2e]">"article.post > h1"</span>
					</div>
				</div>

				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">XPath</h2>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">https://site.com --xpath</span>
						<span class="text-[#ffbd2e]">"//div[@id='main']/p[1]"</span>
					</div>
				</div>
			{:else if currentView === 'Output Formats'}
				<p class="jetbrains-mono-400 mb-6 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					By default, sget outputs raw text. You can format the output using the <code
						>--format</code
					> flag.
				</p>
				<ul class="jetbrains-mono-400 mb-8 space-y-4 text-sm">
					<li class="border border-white/5 bg-[#0f0f0f] p-4">
						<strong class="text-white">JSON:</strong> <code>--format json</code> (Great for piping into
						jq)
					</li>
					<li class="border border-white/5 bg-[#0f0f0f] p-4">
						<strong class="text-white">CSV:</strong> <code>--format csv</code> (Useful for data analysis)
					</li>
					<li class="border border-white/5 bg-[#0f0f0f] p-4">
						<strong class="text-white">Table:</strong> <code>--format table</code> (Human readable terminal
						tables)
					</li>
				</ul>
			{:else if currentView === 'Interactive Mode'}
				<p class="jetbrains-mono-400 mb-6 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					Sometimes you don't know the exact selector you need. Launch sget in Interactive Mode to
					explore the DOM live.
				</p>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">-i https://example.com</span>
					</div>
				</div>
				<p class="jetbrains-mono-400 mb-8 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					This opens a Terminal UI where you can test CSS selectors and instantly see the matched
					results in a split pane.
				</p>
			{:else if currentView === 'Sessions & Auth'}
				<p class="jetbrains-mono-400 mb-6 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					sget handles cookies and sessions automatically if you enable the session manager.
				</p>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#545861]"># Saves cookies to a local session file</span><br />
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white"
							>https://app.com/login -d "user=admin" --session my_session</span
						>
					</div>
				</div>
			{:else if currentView === 'Watch Mode'}
				<p class="jetbrains-mono-400 mb-6 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					Want to track when a price changes or a new article drops? Use watch mode.
				</p>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">https://store.com/item -s ".price" --watch --interval 60s</span
						>
					</div>
				</div>
				<p class="jetbrains-mono-400 mb-8 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					sget will run continuously and only emit output to stdout when the matched content
					changes.
				</p>
			{:else if currentView === 'Configuration'}
				<p class="jetbrains-mono-400 mb-6 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]">
					You can set default behaviors in the config file located at <code
						>~/.config/sget/config.toml</code
					>.
				</p>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm whitespace-pre text-[#8e929a]">
						[default] user_agent = "Mozilla/5.0 (sget CLI)" timeout = 30 retries = 3 [output] color
						= "auto" format = "text"
					</div>
				</div>
			{:else if currentView === 'CLI Reference'}
				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">Global Flags</h2>
				<div class="jetbrains-mono-400 mb-8 overflow-hidden border border-white/10 text-sm">
					<table class="w-full text-left">
						<thead class="border-b border-white/10 bg-[#141414]">
							<tr>
								<th class="p-3 font-normal text-white">Flag</th>
								<th class="p-3 font-normal text-white">Description</th>
							</tr>
						</thead>
						<tbody class="divide-y divide-white/5 text-[#8e929a]">
							<tr>
								<td class="p-3 text-[#00f5c4]">-s, --select</td>
								<td class="p-3">CSS Selector to extract</td>
							</tr>
							<tr>
								<td class="p-3 text-[#00f5c4]">-x, --xpath</td>
								<td class="p-3">XPath expression to extract</td>
							</tr>
							<tr>
								<td class="p-3 text-[#00f5c4]">--attr</td>
								<td class="p-3">Specific HTML attribute to return (e.g. href, src)</td>
							</tr>
							<tr>
								<td class="p-3 text-[#00f5c4]">-i, --interactive</td>
								<td class="p-3">Open the Terminal UI mode</td>
							</tr>
							<tr>
								<td class="p-3 text-[#00f5c4]">-f, --format</td>
								<td class="p-3">Output format (text, json, csv, table)</td>
							</tr>
						</tbody>
					</table>
				</div>
			{:else if currentView === 'Examples'}
				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">Get all image URLs</h2>
				<div class="mb-6 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white">https://gallery.com -s "img" --attr src</span>
					</div>
				</div>

				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">Extract table to CSV</h2>
				<div class="mb-6 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white"
							>https://data.gov/stats -s "table#population" -f csv > output.csv</span
						>
					</div>
				</div>

				<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">Check for 404s on a page</h2>
				<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f]">
					<div class="jetbrains-mono-400 p-4 text-sm">
						<span class="text-[#00f5c4]">sget</span>
						<span class="text-white"
							>https://mysite.com -s "a" --attr href | xargs -n 1 curl -o /dev/null -s -w
							"%&#123;http_code&#125; %&#123;url_effective&#125;\n"</span
						>
					</div>
				</div>
			{/if}

			<div class="mt-16 mb-10 flex items-center justify-between border-t border-white/5 pt-8">
				<div>
					{#if prevPage}
						<button
							onclick={() => setView(prevPage)}
							class="group jetbrains-mono-400 flex flex-col items-start text-sm transition-colors hover:text-white"
						>
							<span class="mb-1 text-xs text-[#545861] transition-colors group-hover:text-[#8e929a]"
								>Previous</span
							>
							<span class="flex items-center gap-2 text-[#8e929a] group-hover:text-white">
								<span class="text-[#00f5c4] transition-transform group-hover:-translate-x-1"
									>&larr;</span
								>
								{prevPage}
							</span>
						</button>
					{/if}
				</div>

				<div class="text-right">
					{#if nextPage}
						<button
							onclick={() => setView(nextPage)}
							class="group jetbrains-mono-400 flex flex-col items-end text-sm transition-colors hover:text-white"
						>
							<span class="mb-1 text-xs text-[#545861] transition-colors group-hover:text-[#8e929a]"
								>Next</span
							>
							<span class="flex items-center gap-2 text-[#8e929a] group-hover:text-white">
								{nextPage}
								<span class="text-[#00f5c4] transition-transform group-hover:translate-x-1"
									>&rarr;</span
								>
							</span>
						</button>
					{/if}
				</div>
			</div>
		</div>
	</main>
</div>
