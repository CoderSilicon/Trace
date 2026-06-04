 <script lang="ts">
  import { fade } from 'svelte/transition';

  // Svelte 5 state
  let copied = $state(false);

  const copyInstallCommand = () => {
    navigator.clipboard.writeText('curl -sL sget.sh | sh');
    copied = true;
    setTimeout(() => (copied = false), 2000);
  };
</script>

<div 
  class="min-h-screen bg-[#060709] text-white selection:bg-[#00f5c4]/30 flex flex-col items-center py-24 font-sans"
>
  <main class="w-full max-w-5xl px-6">
    
    <div class="flex flex-col  items-center text-center mb-16">
      <h1 class="text-4xl md:text-5xl font-bold jetbrains-mono-600 tracking-tight text-white mb-4">
        Install sget
      </h1>
      <p class="text-[#8e929a] text-sm md:text-base flex items-center gap-2 jetbrains-mono-3000">
        Version 1.0.0 <span class="text-white/20">•</span> <a href="#changelog" class="hover:text-white underline decoration-white/20 underline-offset-4 transition-colors">Release Notes</a>
      </p>
    </div>

    <div class="grid grid-cols-1 md:grid-cols-3 gap-6">

      <div class="bg-[#0d0f13] border border-[#00f5c4]/20 rounded-xl p-8 hover:border-[#00f5c4]/50 transition-colors flex flex-col relative overflow-hidden shadow-2xl">
        <div class="absolute top-0 right-0 p-5">
          <span class="text-[10px] font-mono font-bold text-[#060709] bg-[#00f5c4] px-2 py-1 rounded uppercase tracking-wider">Supported</span>
        </div>
        
        <h2 class="text-2xl font-semibold mb-3 jetbrains-mono-500">Linux</h2>
        <p class="text-[#8e929a] text-sm mb-6 grow leading-relaxed jetbrains-mono-300">
          Choose the universal install script for quick setup on Debian, Ubuntu, Arch, RHEL, or Alpine distributions.
        </p>

        <div class="space-y-4 mt-auto">
          <div class="bg-[#060709] border border-white/5 rounded-md p-4 flex items-center overflow-x-auto">
            <code class="font-mono text-sm text-[#00f5c4] whitespace-nowrap">
              <span class="text-[#545861] mr-2">$</span>curl -sL sget.sh | sh
            </code>
          </div>

          <button 
            onclick={copyInstallCommand}
            class="w-full py-2.5 rounded-md border border-white/10 hover:bg-white/5 transition-colors text-sm font-medium flex items-center justify-center gap-2"
          >
            {#if copied}
              <svg class="w-4 h-4 text-[#00f5c4]" fill="none" stroke="currentColor" viewBox="0 0 24 24" transition:fade={{ duration: 150 }}>
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 13l4 4L19 7" />
              </svg>
              <span class="text-[#00f5c4]" transition:fade={{ duration: 150 }}>Copied</span>
            {:else}
              <svg class="w-4 h-4 text-[#8e929a]" fill="none" stroke="currentColor" viewBox="0 0 24 24" transition:fade={{ duration: 150 }}>
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 5H6a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2v-1M8 5a2 2 0 002 2h2a2 2 0 002-2M8 5a2 2 0 012-2h2a2 2 0 012 2m0 0h2a2 2 0 012 2v3m2 4H10m0 0l3-3m-3 3l3 3" />
              </svg>
              <span transition:fade={{ duration: 150 }}>Copy Script</span>
            {/if}
          </button>
        </div>
      </div>

      <div class="bg-[#0d0f13]/80 border border-white/5 rounded-xl p-8 hover:border-white/10 transition-colors flex flex-col relative">
        <div class="absolute top-0 right-0 p-5">
          <span class="text-[10px] font-mono font-bold text-[#eab308] bg-[#eab308]/10 px-2 py-1 rounded uppercase tracking-wider">In Dev</span>
        </div>

        <h2 class="text-2xl font-semibold mb-3 text-white/80 jetbrains-mono-500">Windows</h2>
        <p class="text-[#8e929a] text-sm mb-8 grow leading-relaxed jetbrains-mono-300">
          Native executable target <code class="font-mono text-xs text-white/60 bg-white/5 px-1 py-0.5 rounded">x86_64-windows</code> is scheduled for the next major release.
        </p>

        <div class="mt-auto">
          <div class="w-full py-2.5 rounded-md border border-white/5 bg-white/5 text-white/40 text-sm font-medium flex items-center justify-center cursor-not-allowed jetbrains-mono-200">
            Current Workaround: WSL2
          </div>
        </div>
      </div>

      <div class="bg-[#0d0f13]/40 border border-white/5 rounded-xl p-8 flex flex-col relative">
        <div class="absolute top-0 right-0 p-5">
          <span class="text-[10px] font-mono font-bold text-red-400 bg-red-400/10 px-2 py-1 rounded uppercase tracking-wider">Unsupported</span>
        </div>

        <h2 class="text-2xl font-semibold mb-3 text-white/40 line-through decoration-white/20 jetbrains-mono-500">macOS</h2>
        <p class="text-[#545861] text-sm mb-8 flex-grow leading-relaxed jetbrains-mono-300">
          No native support planned for Apple Silicon or Intel Macs. Target <code class="font-mono text-xs bg-white/5 px-1 py-0.5 rounded">darwin</code> is unsupported.
        </p>

        <div class="mt-auto">
          <div class="w-full py-2.5 rounded-md border border-white/5 bg-transparent text-white/20 text-sm font-medium flex items-center justify-center cursor-not-allowed jetbrains-mono-200">
            Not Available
          </div>
        </div>
      </div>

    </div>
  </main>
</div> 

