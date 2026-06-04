<script lang="ts">
	// import { resolve } from '$app/paths';
	// Terminal simulated output lines

	const features = [
		{
			id: 'interactive-mode',
			title: 'Visuals',
			description: 'See the output in colorfull way and output which matters.',
			icon: '<path stroke-linecap="round" stroke-linejoin="round" d="M13 10V3L4 14h7v7l9-11h-7z" />'
		},
		{
			id: 'structured-output',
			title: 'Structured Output',
			description: 'Get clean JSON output; with all the necessary infos required',
			icon: '<path stroke-linecap="round" stroke-linejoin="round" d="M20 7l-8-4-8 4m16 0l-8 4m8-4v10l-8 4m0-10L4 7m8 4v10M4 7v10l8 4" />'
		},
		{
			id: 'smart-selectors',
			title: 'Simpilicity',
			description: 'Multiple flags which help you get what you "really" want',
			icon: '<path stroke-linecap="round" stroke-linejoin="round" d="M3 4a1 1 0 011-1h16a1 1 0 011 1v2.586a1 1 0 01-.293.707l-6.414 6.414a1 1 0 00-.293.707V17l-4 4v-6.586a1 1 0 00-.293-.707L3.293 7.293A1 1 0 013 6.586V4z" />'
		},
		{
			id: 'visual-diffs',
			title: 'Built-in File Support',
			description: 'Extracted data can be easily piped and stored into a file with just one flag.',
			icon: '<path stroke-linecap="round" stroke-linejoin="round" d="M9 19v-6a2 2 0 00-2-2H5a2 2 0 00-2 2v6a2 2 0 002 2h2a2 2 0 002-2zm0 0V9a2 2 0 012-2h2a2 2 0 012 2v10m-6 0a2 2 0 002 2h2a2 2 0 002-2m0 0V5a2 2 0 012-2h2a2 2 0 012 2v14a2 2 0 01-2 2h-2a2 2 0 01-2-2z" />'
		}
	];

	const bulletPoints = [
		{ id: 'selectors', text: 'CSS and XPath selectors' },
		{ id: 'output', text: 'JSON, CSV, and table output' },
		{ id: 'redirects', text: 'Follow redirects & handle auth' },
		{ id: 'caching', text: 'Built-in caching & rate limiting' }
	];

	const codeExamples = [
		{
			id: 1,
			comment: 'Fetch the metadata of a webpage',
			command: 'sget --info https://archlinux.org'
		},
		{
			id: 2,
			comment: 'Extract only the page title',
			command: 'sget --links https://archlinux.org'
		},
		{
			id: 3,
			comment: 'Write output to a file',
			command: 'sget --json https://archlinux.org -o icon.png'
		},
		{
			id: 4,
			comment: 'Output the raw JSON content directly',
			command: 'sget --json https://archlinux.org'
		}
	];
	// Code examples for the right column box
	const terminalLines = [
		{
			html: '<span class="text-gray-400">user@archlinux:<span class="text-blue-400">~</span>$</span> <span class="text-[#e5c07b]">sget</span> --info https://archlinux.org'
		},
		{ html: '' },
		{ html: '<span class="text-[#00e5ff] font-semibold">=== Inspected Data ===</span>' },
		{ html: '<span class="text-[#d16dff]">[ ]</span>' },
		{
			html: '<span class="text-white">URL:            </span><span class="text-gray-400">https://archlinux.org</span>'
		},
		{
			html: '<span class="text-white">Status Code:    </span><span class="text-[#27c93f] font-semibold">200</span>'
		},
		{
			html: '<span class="text-white">Server:         </span><span class="text-gray-300">nginx</span>'
		},
		{
			html: '<span class="text-white">Content-Type:   </span><span class="text-gray-300">text/html; charset=utf-8</span>'
		},
		{
			html: '<span class="text-white">Page Size:      </span><span class="text-[#27c93f]">25030 bytes</span>'
		},
		{ html: '' },
		{ html: '<span class="text-[#d16dff]">[ METADATA ]</span>' },
		{
			html: '<span class="text-white">Title:          </span><span class="text-[#d19a66]">Arch Linux</span>'
		},
		{
			html: '<span class="text-white">Description:    </span><span class="text-gray-400">Not found</span>'
		},
		{
			html: '<span class="text-white">Logo/Favicon:   </span><span class="text-gray-400">/static/archlinux_common_style/favicon.png</span>'
		}
	];
</script>

<section class="relative min-h-screen bg-[#060709] text-white">
	<main class="mt-16 flex flex-col items-center px-5 text-center">
		<h1
			class="jetbrains-mono-400 mb-6 max-w-4xl font-sans text-[2.5rem] leading-[1.15] font-extrabold tracking-tight md:text-[4.2rem]"
		>
			Extract insights from <span class="text-[#004b39]">any website</span>
		</h1>

		<p class="jetbrains-mono-300 mb-10 max-w-2xl text-[1.05rem] leading-relaxed text-[#8e929a]">
			sget is a modern command-line tool for web data extraction.<br class="hidden md:inline" />
			Like curl, but interactive, visual, and built for humans.
		</p>

		<div class="mb-16 flex gap-4">
			<button
				class="jetbrains-mono-600 flex items-center gap-2 rounded bg-[#014e44] px-7 py-3.5 text-base font-bold text-zinc-100 transition-opacity hover:bg-[#014e44b4] hover:opacity-90"
			>
				Get Started
			</button>
			<button
				class="jetbrains-mono-200 rounded border border-white/10 bg-transparent px-7 py-3.5 text-base font-medium text-[#8e929a] transition-all hover:border-white/30 hover:text-white"
			>
				Read the docs
			</button>
		</div>

		<div class="w-full max-w-170 overflow-hidden bg-[#0d0f13] text-left shadow-2xl">
			<div class="border-bottom relative flex items-center bg-[#13161c] px-4 py-3">
				<div class="z-10 flex gap-2">
					<span class="h-2.5 w-2.5 rounded-full bg-[#ff5f56]"></span>
					<span class="h-2.5 w-2.5 rounded-full bg-[#ffbd2e]"></span>
					<span class="h-2.5 w-2.5 rounded-full bg-[#27c93f]"></span>
				</div>
				<div
					class="absolute inset-0 flex w-full items-center justify-center text-center text-xs text-[#61656e]"
				>
					sget
				</div>
			</div>

			<div
				class="bg-[#060709] p-6 font-mono text-[0.9rem] leading-6 tracking-wide whitespace-pre-wrap"
			>
				{#each terminalLines as line, i (i)}
					<p class="min-h-6">
						{@html line.html}
					</p>
				{/each}
			</div>
		</div>
	</main>
</section>

<section
	class="flex justify-center bg-zinc-950 px-6 py-24 text-white selection:bg-[#00f5c4]/30 md:px-12"
>
	<div class="w-full max-w-5xl">
		<!-- Header Section -->
		<div class="mb-20 max-w-2xl">
			<h2 class="jetbrains-mono-400 mb-6 text-3xl font-bold tracking-tight md:text-4xl">
				<span class="text-white">sget</span> is a frontend built on libcurl.
			</h2>
			<p class="jetbrains-mono-300 font-mono text-sm leading-relaxed text-[#8e929a] md:text-base">
				Built for users who want performance, simplicity, and only what they need; without complex
				notations.
			</p>
		</div>

		<!-- Features Grid -->
		<div class="grid grid-cols-1 gap-x-16 gap-y-12 md:grid-cols-2">
			{#each features as feature (feature.id)}
				<!-- 
                  Removed the background card styling. 
                  Using a subtle top border and padding for structure. 
                -->
				<div
					class="group border-t border-white/5 pt-6 transition-colors duration-300 hover:border-white/20"
				>
					<div class="mb-3 flex items-center gap-3">
						<!-- Naked icon without the background box -->
						<div
							class="text-[#00f5c4] transition-transform duration-300 group-hover:-translate-y-0.5"
						>
							<svg
								xmlns="http://www.w3.org/2000/svg"
								class="h-5 w-5"
								fill="none"
								viewBox="0 0 24 24"
								stroke="currentColor"
								stroke-width="1.75"
							>
								{@html feature.icon}
							</svg>
						</div>

						<h3 class="font-mono text-[15px] font-medium tracking-wide text-zinc-100 jetbrains-mono-500">
							{feature.title}
						</h3>
					</div>

					<p class="text-sm leading-relaxed text-[#61656e] jetbrains-mono-300">
						{feature.description}
					</p>
				</div>
			{/each}
		</div>
	</div>
</section>

<section
	class="flex justify-center bg-[#060709] px-6 py-24 text-white selection:bg-zinc-700/50 md:px-12"
>
	<div class="grid w-full max-w-5xl grid-cols-1 items-start gap-16 lg:grid-cols-12 lg:gap-12">
		<div class="space-y-8 lg:col-span-5 lg:pt-4">
			<div>
				<h2
					class="jetbrains-mono-400 text-3xl leading-tight font-bold tracking-tight text-zinc-100 md:text-4xl"
				>
					Simple syntax, <br class="hidden sm:block" />
					<span class="text-[#00f5c4]">powerful results.</span>
				</h2>

				<p class="mt-5 max-w-lg jetbrains-mono-300 text-sm leading-relaxed text-[#61656e] md:text-base">
					Extract exactly what you need. No need to memorize cryptic flags.
				</p>
			</div>

			<ul class="space-y-4 pt-4 jetbrains-mono-300 text-sm md:text-base">
				{#each bulletPoints as point (point.id)}
					<li class="flex items-start text-zinc-400">
						<span class="mt-2.5 mr-4 inline-block h-1 w-1 shrink-0 rounded-full bg-zinc-600"></span>
						<span class="leading-relaxed">{point.text}</span>
					</li>
				{/each}
			</ul>
		</div>

		<div class="lg:col-span-7">
			<div
				class="w-full space-y-10 border-l border-white/10 py-2 pl-6 font-mono text-sm md:pl-10 md:text-[0.95rem]"
			>
				{#each codeExamples as example (example.id)}
					<div class="group space-y-2">
						<p
							class="text-zinc-500 transition-colors duration-300 select-none group-hover:text-zinc-400"
						>
							// {example.comment}
						</p>
						<p class="leading-relaxed break-all text-zinc-200">
							<span class="mr-3 text-zinc-600 select-none">❯</span>{example.command}
						</p>
					</div>
				{/each}
			</div>
		</div>
	</div>
</section>

<section
	class="jetbrains-mono-400 flex h-screen flex-col items-center justify-center bg-[#060709] px-6 pt-32 text-white selection:bg-[#00f5c4]/30 md:px-12"
>
	<div class="mb-36 flex max-w-2xl flex-col items-center text-center">
		<h2 class="jetbrains-mono-600 mb-5 text-3xl font-bold tracking-tight md:text-4xl">
			Ready to fetch?
		</h2>
		<p class="jetbrains-mono-300 mb-8 max-w-md text-sm leading-relaxed text-[#8e929a] md:text-base">
			Get started in seconds. Works on macOS, Linux, and Windows.
		</p>
		<button
			class="jetbrains-mono-300 flex items-center gap-2 rounded bg-[#002e23e5] px-7 py-3.5 font-sans text-base font-bold text-[#f8f8f8] transition-opacity hover:opacity-90"
		>
			Download sget <span class="text-lg">&rarr;</span>
		</button>
	</div>
</section>
