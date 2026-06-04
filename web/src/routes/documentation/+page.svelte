<!-- <script lang="ts">
	// Svelte 5 State
	let currentView = $state('Introduction');

	// Navigation Structure
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

	// Clever Scalable Data Engine for Doc Content
	const docsContent: Record<string, any[]> = {
		Introduction: [
			{
				type: 'p',
				text: 'sget is a command-line tool for extracting data from websites. Think of it as curl with superpowers: CSS selectors, structured output, interactive mode, and visual diffs, all in a single binary.'
			},
			{ type: 'h2', text: 'Why sget?' },
			{ type: 'p', text: "If you've ever written a command like this:" },
			{
				type: 'code',
				lang: 'bash',
				code: '<span class="text-[#00f5c4]">curl</span> <span class="text-white">-s</span> https://example.com | <span class="text-[#00f5c4]">grep</span> <span class="text-[#ffbd2e]">\'&lt;title&gt;\'</span> | <span class="text-[#00f5c4]">sed</span> <span class="text-[#ffbd2e]">\'s/&lt;[^&gt;]*&gt;//g\'</span>'
			},
			{ type: 'p', text: 'Then sget is for you. The same thing with sget:' },
			{
				type: 'code',
				lang: 'bash',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://example.com --select</span> <span class="text-[#ffbd2e]">"title"</span> <span class="text-white">--text</span>'
			},
			{ type: 'h2', text: 'Features' },
			{
				type: 'list',
				items: [
					'CSS and XPath selectors for precise element targeting',
					'JSON, CSV, and table output formats',
					'Interactive TUI for exploring pages',
					'Session management with cookie persistence',
					'Watch mode for monitoring changes',
					'Built-in rate limiting and retry logic',
					'Single binary, zero dependencies'
				]
			}
		],
		Installation: [
			{
				type: 'p',
				text: 'sget is distributed as a single standalone binary. You can install it using your favorite package manager or download it directly.'
			},
			{ type: 'h2-sub', text: 'macOS / Linux (Homebrew)' },
			{ type: 'code', code: '<span class="text-white">brew install sget-cli</span>' },
			{ type: 'h2-sub', text: 'Windows (Scoop)' },
			{ type: 'code', code: '<span class="text-white">scoop install sget</span>' },
			{ type: 'h2-sub', text: 'From Source (Cargo)' },
			{ type: 'code', code: '<span class="text-white">cargo install sget-cli</span>' }
		],
		'Quick Start': [
			{
				type: 'p',
				text: "Let's extract all the headline links from Hacker News in one simple command."
			},
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://news.ycombinator.com -s</span> <span class="text-[#ffbd2e]">".titleline > a"</span> <span class="text-white">--attr href</span>'
			},
			{
				type: 'p',
				text: 'This command fetches the page, parses the DOM, finds all anchor tags inside elements with the class <code class="rounded bg-white/5 px-1.5 py-0.5 text-white">titleline</code>, and prints out their <code class="rounded bg-white/5 px-1.5 py-0.5 text-white">href</code> attributes.'
			}
		],
		'Basic Usage': [
			{
				type: 'p',
				text: 'The basic syntax of sget is designed to be intuitive. You provide a URL and tell sget what you want to extract.'
			},
			{
				type: 'code',
				code: '<span class="text-[#545861]"># General Syntax</span><br /><span class="text-[#00f5c4]">sget</span> <span class="text-white">[URL] [OPTIONS]</span>'
			},
			{ type: 'h2-sub', text: 'Sending Headers' },
			{
				type: 'p',
				text: 'Pass headers using the <code class="rounded bg-white/5 px-1.5 py-0.5 text-white">-H</code> flag, exactly like curl:'
			},
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://api.github.com -H</span> <span class="text-[#ffbd2e]">"Authorization: Bearer TOKEN"</span>'
			}
		],
		Selectors: [
			{ type: 'p', text: 'sget supports both CSS and XPath selectors natively.' },
			{ type: 'h2-sub', text: 'CSS Selectors' },
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://site.com --select</span> <span class="text-[#ffbd2e]">"article.post > h1"</span>'
			},
			{ type: 'h2-sub', text: 'XPath' },
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://site.com --xpath</span> <span class="text-[#ffbd2e]">"//div[@id=\'main\']/p[1]"</span>'
			}
		],
		'Output Formats': [
			{
				type: 'p',
				text: 'By default, sget outputs raw text. You can format the output using the <code class="rounded bg-white/5 px-1.5 py-0.5 text-white">--format</code> flag.'
			},
			{
				type: 'styled-cards',
				items: [
					{ label: 'JSON:', desc: '<code>--format json</code> (Great for piping into jq)' },
					{ label: 'CSV:', desc: '<code>--format csv</code> (Useful for data analysis)' },
					{ label: 'Table:', desc: '<code>--format table</code> (Human readable terminal tables)' }
				]
			}
		],
		'Interactive Mode': [
			{
				type: 'p',
				text: "Sometimes you don't know the exact selector you need. Launch sget in Interactive Mode to explore the DOM live."
			},
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">-i https://example.com</span>'
			},
			{
				type: 'p',
				text: 'This opens a Terminal UI where you can test CSS selectors and instantly see the matched results in a split pane.'
			}
		],
		'Sessions & Auth': [
			{
				type: 'p',
				text: 'sget handles cookies and sessions automatically if you enable the session manager.'
			},
			{
				type: 'code',
				code: '<span class="text-[#545861]"># Saves cookies to a local session file</span><br /><span class="text-[#00f5c4]">sget</span> <span class="text-white">https://app.com/login -d "user=admin" --session my_session</span>'
			}
		],
		'Watch Mode': [
			{
				type: 'p',
				text: 'Want to track when a price changes or a new article drops? Use watch mode.'
			},
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://store.com/item -s ".price" --watch --interval 60s</span>'
			},
			{
				type: 'p',
				text: 'sget will run continuously and only emit output to stdout when the matched content changes.'
			}
		],
		Configuration: [
			{
				type: 'p',
				text: 'You can set default behaviors in the config file located at <code class="rounded bg-white/5 px-1.5 py-0.5 text-white">~/.config/sget/config.toml</code>.'
			},
			{
				type: 'code',
				code: '<span class="text-[#8e929a]">[default]\nuser_agent = "Mozilla/5.0 (sget CLI)"\ntimeout = 30\nretries = 3\n\n[output]\ncolor = "auto"\nformat = "text"</span>'
			}
		],
		'CLI Reference': [
			{ type: 'h2-sub', text: 'Extraction Flags' },
			{
				type: 'table',
				headers: ['Flag', 'Description'],
				rows: [
					['-s, --select', 'CSS Selector to extract'],
					['-x, --xpath', 'XPath expression to extract'],
					['--attr', 'Specific HTML attribute to return (e.g. href, src)'],
					['-j, --json', 'Output result in JSON format'],
					['-c, --csv', 'Output result in CSV format'],
					['-t, --table', 'Output result in a terminal table'],
					['-o, --output', 'Write output to a file instead of stdout']
				]
			},
			{ type: 'h2-sub', text: 'Operational & Global Flags' },
			{
				type: 'table',
				headers: ['Flag', 'Description'],
				rows: [
					['-i, --info', 'Show detailed info about the target URL'],
					['-bi, --briefInfo', 'Show summary info about the target URL'],
					['-l, --links', 'List all links found on the target page'],
					['--author', 'Display project author information'],
					['--version', 'Display the current version of sget']
				]
			}
		],
		Examples: [
			{ type: 'h2-sub', text: 'Get all image URLs' },
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://gallery.com -s "img" --attr src</span>'
			},
			{ type: 'h2-sub', text: 'Extract table to CSV' },
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://data.gov/stats -s "table#population" -f csv > output.csv</span>'
			},
			{ type: 'h2-sub', text: 'Check for 404s on a page' },
			{
				type: 'code',
				code: '<span class="text-[#00f5c4]">sget</span> <span class="text-white">https://mysite.com -s "a" --attr href | xargs -n 1 curl -o /dev/null -s -w "%&#123;http_code&#125; %&#123;url_effective&#125;\n"</span>'
			}
		]
	};

	// Svelte 5 Derived Runes for Next/Prev pagination
	const allLinks = navSections.flatMap((s) => s.links);
	let currentIndex = $derived(allLinks.indexOf(currentView));
	let prevPage = $derived(currentIndex > 0 ? allLinks[currentIndex - 1] : null);
	let nextPage = $derived(currentIndex < allLinks.length - 1 ? allLinks[currentIndex + 1] : null);

	function setView(view: string) {
		currentView = view;
		window.scrollTo({ top: 0, behavior: 'smooth' });
	}
</script>

<div
	class="relative flex min-h-screen flex-col bg-[#0a0a0a] text-white selection:bg-[#00f5c4]/30 md:flex-row"
>
	<aside
		class="hidden h-full shrink-0 border-r border-white/5 bg-[#0a0a0a] p-6 md:sticky md:top-0 md:block md:w-72 md:p-8"
	>
		<div class="jetbrains-mono-700 mb-10 text-lg tracking-tight">docs</div>

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

	<main class="min-w-0 flex-1 px-6 py-12 md:px-12 md:py-16 lg:px-20">
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

			{#if docsContent[currentView]}
				{#each docsContent[currentView] as block}
					{#if block.type === 'p'}
						<p
							class="jetbrains-mono-400 mb-10 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]"
						>
							{@html block.text}
						</p>
					{:else if block.type === 'h2'}
						<h2 class="jetbrains-mono-700 mb-4 text-xl text-white">
							{block.text}
						</h2>
					{:else if block.type === 'h2-sub'}
						<h2 class="jetbrains-mono-700 mb-4 text-lg text-white">
							{block.text}
						</h2>
					{:else if block.type === 'code'}
						<div class="mb-8 overflow-hidden border border-white/10 bg-[#0f0f0f] shadow-lg">
							{#if block.lang}
								<div
									class="flex items-center justify-between border-b border-white/5 bg-white/2 px-4 py-2"
								>
									<span class="jetbrains-mono-400 text-xs text-[#545861]">{block.lang}</span>
								</div>
							{/if}
							<div class="jetbrains-mono-400 overflow-x-auto p-4 text-sm whitespace-nowrap">
								{@html block.code}
							</div>
						</div>
					{:else if block.type === 'list'}
						<ul class="mb-16 space-y-3.5">
							{#each block.items as item}
								<li class="flex items-start">
									<span class="mt-2 mr-3.5 inline-block h-1.5 w-1.5 shrink-0 bg-[#00f5c4]"></span>
									<span
										class="jetbrains-mono-400 text-sm leading-relaxed text-[#8e929a] md:text-[0.95rem]"
									>
										{item}
									</span>
								</li>
							{/each}
						</ul>
					{:else if block.type === 'styled-cards'}
						<ul class="jetbrains-mono-400 mb-8 space-y-4 text-sm">
							{#each block.items as item}
								<li class="border border-white/5 bg-[#0f0f0f] p-4">
									<strong class="text-white">{item.label}</strong>
									{@html item.desc}
								</li>
							{/each}
						</ul>
					{:else if block.type === 'table'}
						<div class="jetbrains-mono-400 mb-8 overflow-hidden border border-white/10 text-sm">
							<table class="w-full text-left">
								<thead class="border-b border-white/10 bg-[#141414]">
									<tr>
										{#each block.headers as header}
											<th class="p-3 font-normal text-white">{header}</th>
										{/each}
									</tr>
								</thead>
								<tbody class="divide-y divide-white/5 text-[#8e929a]">
									{#each block.rows as row}
										<tr>
											<td class="p-3 text-[#00f5c4]">{row[0]}</td>
											<td class="p-3">{@html row[1]}</td>
										</tr>
									{/each}
								</tbody>
							</table>
						</div>
					{/if}
				{/each}
			{/if}

			<div class="mt-16 mb-10 flex items-center justify-between border-t border-white/5 pt-8">
				<div>
					{#if prevPage}
						<button
							onclick={() => setView(prevPage)}
							class="group jetbrains-mono-400 flex flex-col items-start text-sm transition-colors hover:text-white"
						>
							<span
								class="mb-1 text-xs text-[#545861] transition-colors group-hover:text-[#8e929a]"
							>
								Previous
							</span>
							<span class="flex items-center gap-2 text-[#8e929a] group-hover:text-white">
								<span class="text-[#00f5c4] transition-transform group-hover:-translate-x-1">
									&larr;
								</span>
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
							<span
								class="mb-1 text-xs text-[#545861] transition-colors group-hover:text-[#8e929a]"
							>
								Next
							</span>
							<span class="flex items-center gap-2 text-[#8e929a] group-hover:text-white">
								{nextPage}
								<span class="text-[#00f5c4] transition-transform group-hover:translate-x-1">
									&rarr;
								</span>
							</span>
						</button>
					{/if}
				</div>
			</div>
		</div>
	</main>
</div> -->
<section class="min-h-screen bg-[#0a0a0a] py-12 text-white flex items-center">
	<div class="container mx-auto px-4">
		<h2 class="mb-8 text-center text-3xl font-bold jetbrains-mono-600">Documentation will be available soon</h2>
		<p class="mx-auto mb-12 max-w-2xl text-center text-[#8e929a] jetbrains-mono-300 text-sm leading-relaxed">
			The sget cli is currently in early development, and the documentation is being actively worked
			on. In the meantime, you can check out the <a
				href="/documentation"
				class="text-[#00f5c4] hover:underline">documentation</a
			>, and you will be redirected to this page only. Basically the version is alpha and the
			documentation is not ready yet. Please check back later for updates.
		</p>
	</div>
</section>
