import { useEffect, useState } from "react";

export default function App() {
    const [count, setCount] = useState(1);

    function handleClick() {
        setCount(count + 1);
    }

    return (
        <div>
            <p>
                Hello!!!
            </p>
            <br />
            <br />
            <Pong />
            <br />
            <br />
            <TestButton />
            <br />
            <TestButton startVal={5} />
            <br />
            <SharedButton count={count} onClick={handleClick} />
            <br />
            <SharedButton count={count} onClick={handleClick} />
        </div>
    )
}

function Pong() {
    const [pong, setPong] = useState<string | null>(null);

    useEffect(() => {
        fetch("/api/ping")
        .then((r) => r.json())
        .then((data) => setPong(data.message + " @ " + data.time))
        .catch((err) => setPong("error: " + err.message));
    }, []);

    return (
        <p>
            Backend says: {pong ?? "loading..."}
        </p>
    )
}

type SharedButtonProps = {
    count: number,
    onClick: () => void
}

function SharedButton({ count, onClick }: SharedButtonProps) {
    return (
        <button onClick={onClick}>
            Clicked {count} times
        </button>
    )
}

type TestButtonProps = {
    startVal?: number
}

function TestButton({ startVal = 0 }: TestButtonProps) {
    const [count, setCount] = useState(startVal);

    function handleClick() {
        setCount(count + 1);
    }

    return (
        <button onClick={handleClick}>
            {count}
        </button>
    )
}