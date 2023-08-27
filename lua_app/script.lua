


function test_call(a, b)
	print("a : ", a)
	print("b : ", b)
	print("myTable ", myTable)
	print("myTable key1: ", myTable.key1)
	return a + b
end

width = 100;
heigh = 120;

--background = {red = 0.3, green = 1.20, blue = 0}
--background = "RED"

local function _test_co()
	local co = coroutine.wrap(function()
			print("___coroutine.wrap__begin___")
			--print(pcall(coroutine.yield))
			local ret = corutine_pcall("")
			print("_______co ret____", ret)
			print("___coroutine.yield__begein___")
			--coroutine.yield()
			print("___coroutine.wrap__after___")
			--corutine_pcall("")
		end)
	print("co ", co, type(co))
	co()
	--local thread, isMain = coroutine.running()
	--print("thread ", thread, " is main", isMain)
	--coroutine.resume(thread)
	--co()
	--corutine_pcall()
end

local function _test_dir()
	local ret = read_dir(nil)
	print("ret: ", ret)
	for k, path in pairs(ret) do
		print("k: ", k)
		print("path: ", path)
	end
end

local function _test_lib()
	package.path = package.path .. ";E:/cpp_project/lua_util/lib/lib"
	package.cpath = package.path .. "?.lib"
	local mylib = require("mylib")
	print("mylib: ", mylib)
	--mylib.dir("dir")
	--print("mylib dir")
end

local tab =
{
	1,2,3,4,5,6
}

local function _test_map()
	l_map(tab, function(i) return i*10 end)
	for k, v in pairs(tab) do
		print("k: ", k)
		print("v: ", v)
	end
end

local function _test_split()
	local list = l_split("1|20|34|55|11", '|')
	for k, v in pairs(list) do
		print("k: ", k)
		print("v: ", v)
	end
end

local function _test_upper()
	local ret = l_upper('abcdefeg')
	print("ret: ", ret)
end

local function _test_tconcat()
	local t = {1,2,3,'s','f','q','w','e','f'}
	local ret = l_tconcat(t)
	print("ret: ", ret)
end

local function xcounter()
  local n = 1
  local function counter(n)
      return 1+n
  end
  return counter(n)
end

function test_lua()
	print("_________tesdt_lua____")
	--_test_lib()
	--_test_map()
	--_test_split()
	--_test_upper()
	--_test_tconcat()
	--print("register: ", register)

	--local func = new_counter()
	--print("new_counter ret: ", func())
	--print("xcounter: ", xcounter())
	local x = tuple.new(10, "hi", {}, 3)
	print(x(1))
	print(x(2))
	print(x())
	--print(RED.red)
end

function f(x, y)
	print("_____f________")
	return (x^2 * math.sin(y)) / (1 - x);
end