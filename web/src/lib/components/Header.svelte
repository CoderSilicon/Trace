<script lang="ts">
    import { resolve } from '$app/paths';
    import { page } from '$app/state';
    import logo from '$lib/assets/favicon.png';

    const navLinks = [
        { name: 'Home', href: '/'},
        { name: 'About', href: '/about' },
        { name: 'Docs', href: '/documentation' },
        { name: 'Downloads', href: '/download' }
    ] as const;

    let isMenuOpen = $state(false);

    function toggleMenu() {
        isMenuOpen = !isMenuOpen;
    }
</script>

<header class="relative w-full transition-all duration-300 bg-transparent">
    <div class="mx-auto flex max-w-6xl items-center justify-between px-10 py-4">
        <div class="text-xl font-bold tracking-tight">
            <img src={logo} alt="Logo" class="h-10 w-10 rounded-none">
        </div>

        <nav class="hidden md:flex items-center gap-8">
            {#each navLinks as link, i (i)}
                {@const isActive = page.url.pathname === link.href}
                <a
                    href={resolve(link.href)}
                    class="text-sm transition-colors duration-200 jetbrains-mono-400 {isActive
                        ? 'text-white'
                        : 'text-[#8e929a] hover:text-white'}"
                >
                    {link.name}
                </a>
            {/each}
            <a
                href={resolve("/contact")}
                class="bg-[#00f5c4] rounded-none jetbrains-mono-600 px-6 py-2.5 text-sm font-bold text-[#0a0a0a] transition-colors hover:bg-white"
            >
                Get in touch
            </a>
        </nav>

        <button
            class="flex flex-col gap-1.5 p-2 md:hidden rounded-none"
            onclick={toggleMenu}
            aria-label="Toggle menu"
        >
            <div class="h-0.5 w-6 bg-white transition-transform duration-300 {isMenuOpen ? 'translate-y-2 rotate-45' : ''}"></div>
            <div class="h-0.5 w-6 bg-white transition-opacity duration-300 {isMenuOpen ? 'opacity-0' : ''}"></div>
            <div class="h-0.5 w-6 bg-white transition-transform duration-300 {isMenuOpen ? '-translate-y-2 -rotate-45' : ''}"></div>
        </button>
    </div>

    {#if isMenuOpen}
        <nav class="absolute left-0 w-full border-t border-zinc-800 z-50 bg-[#0a0a0a] px-10 py-6 shadow-2xl md:hidden">
            <div class="flex flex-col gap-6">
                {#each navLinks as link, i (i)}
                    {@const isActive = page.url.pathname === link.href}
                    <a
                        href={resolve(link.href)}
                        onclick={toggleMenu}
                        class="text-base transition-colors duration-200 jetbrains-mono-400 {isActive
                            ? 'text-white'
                            : 'text-[#8e929a] hover:text-white'}"
                    >
                        {link.name}
                    </a>
                {/each}
                <a
                    href={resolve("/contact")}
                    onclick={toggleMenu}
                    class="mt-4 bg-[#00f5c4] rounded-none px-5 py-3 text-center text-base font-bold text-[#0a0a0a] transition-colors hover:bg-white jetbrains-mono-600"
                >
                    Get in touch
                </a>
            </div>
        </nav>
    {/if}
</header>