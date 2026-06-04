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

    let scrollY = $state(0);
    let isMenuOpen = $state(false);

    function toggleMenu() {
        isMenuOpen = !isMenuOpen;
    }
</script>

<svelte:window bind:scrollY />

<header 
    class="sticky top-0 z-50 w-full transition-all duration-300 {scrollY > 20 ? 'bg-[#0a0a0a]/80 backdrop-blur-md border-b border-white/5' : 'bg-transparent'}"
>
    <div class="mx-auto flex max-w-6xl items-center justify-between px-10 py-3">
        <div class="text-xl font-bold tracking-tight">
            <img src={logo} alt="Logo" class="h-12 w-12">
        </div>

        <nav class="hidden md:flex items-center gap-8">
            {#each navLinks as link, i (i)}
                {@const isActive = page.url.pathname === link.href}
                <a
                    href={resolve(link.href)}
                    class=" text-sm transition-colors duration-200 jetbrains-mono-400 {isActive
                        ? 'text-[#0b4b3e]'
                        : 'text-[#8e929a] hover:text-white'}"
                >
                    {link.name}
                </a>
            {/each}
            <a
                href={resolve("/contact")}
                class="bg-[#0b5849] rounded-xl jetbrains-mono-600 px-5.5 py-2.5 text-sm font-bold text-zinc-100 transition-opacity hover:opacity-90"
            >
                Get in touch
            </a>
        </nav>

        <button
            class="flex flex-col gap-1.5 p-2 md:hidden"
            onclick={toggleMenu}
            aria-label="Toggle menu"
        >
            <div class="h-0.5 w-6 bg-white transition-transform duration-300 {isMenuOpen ? 'translate-y-2 rotate-45' : ''}"></div>
            <div class="h-0.5 w-6 bg-white transition-opacity duration-300 {isMenuOpen ? 'opacity-0' : ''}"></div>
            <div class="h-0.5 w-6 bg-white transition-transform duration-300 {isMenuOpen ? '-translate-y-2 -rotate-45' : ''}"></div>
        </button>
    </div>

    {#if isMenuOpen}
        <nav class="absolute left-0 w-full border-t border-white/5 bg-[#0a0a0a]/95 px-10 py-6 shadow-2xl backdrop-blur-xl md:hidden">
            <div class="flex flex-col gap-6">
                {#each navLinks as link, i (i)}
                    {@const isActive = page.url.pathname === link.href}
                    <a
                        href={resolve(link.href)}
                        onclick={toggleMenu}
                        class="font-sans text-base transition-colors duration-200 jetbrains-mono-400 {isActive
                            ? 'text-[#00f5c4]'
                            : 'text-[#8e929a] hover:text-white'}"
                    >
                        {link.name}
                    </a>
                {/each}
                <a
                    href={resolve("/contact")}
                    onclick={toggleMenu}
                    class="mt-2 bg-[#00f5c4] px-5 py-3 text-center font-sans text-base font-bold text-[#0a0a0a] transition-opacity hover:opacity-90"
                >
                    Get in touch
                </a>
            </div>
        </nav>
    {/if}
</header>