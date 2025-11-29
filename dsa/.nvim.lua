if nil then
	vim.api.nvim_create_autocmd ("FileType", {
		pattern = { "cpp" },
		callback = function (e)
			vim.keymap.set ("n", "<leader>r", function ()
				local f = vim.fn.getreg ("%")
				vim.cmd (
					":w | sp | set nonu nornu " ..
					"| term clang++ -std=c++20 -pedantic-errors -Werror=pedantic -Wall -Wextra " ..
					f .. " && ./a.out"
				)
			end, { noremap = true, silent = true, buffer = e.buf})
		end,
	})
end

vim.cmd ('source /desktop/nvim/qmake/exrc')
